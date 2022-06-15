#include "server.h"
#include <fcntl.h>
#include <iostream>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include "util/crypto/random.h"
#include "util/crypto/sha256.h"
#include "util/crypto/key.h"
#include "network/connection_manager.h"
#include "config/env_manager.h"
#include <boost/exception/all.hpp>
#include "service/http_request_service/http_server_service.h"
#include "service/http_request_service/rest_api_service.h"
#include "service/node_monitor_service/node_monitor_service.h"
#include "service/node_request_service/node_request_service.h"
#include "service/peer_request_service/p2p_net_service.h"
#include "service/peer_request_service/p2p_lan_service.h"
#include "task/detail/VxlanManager.h"
#include "util/system_info.h"
#include "task/HttpDBCChainClient.h"
#include "task/detail/image/ImageManager.h"

static std::unique_ptr<ECCVerifyHandle> g_ecc_verify_handle;
static std::unique_ptr<std::recursive_mutex[]> g_ssl_lock;

void ssl_locking_callback(int mode, int type, const char *file, int line)
{
    if (mode & CRYPTO_LOCK)
    {
        g_ssl_lock[type].lock();
    }
    else
    {
        g_ssl_lock[type].unlock();
    }
}

NODE_TYPE Server::NodeType = NODE_TYPE::ComputeNode;
std::string Server::NodeName = "";

// main函数中的第一步，初始化
ERRCODE Server::Init(int argc, char *argv[]) {
    ERRCODE err = ERR_SUCCESS;

    // 初始化一些参数（NodeType,Nodename），及是否移到后台执行
    err = ParseCommandLine(argc, argv);
    if (ERR_SUCCESS != err) {
        return err;
    }

    // 初始化log设置
    err = dbclog::instance().init();
    if (ERR_SUCCESS != err) {
        return 0;
    }

    std::cout << "begin server init ..." << std::endl;

    // Crypto
    // 初始化openssl随机数
    err = InitCrypto();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init crypto failed";
        return err;
    }

    // 初始化一些环境变量，如位置变量，配置文件位置啥的
    // EnvManager
    std::cout << "begin to init EvnManager" << std::endl;
    err = EnvManager::instance().Init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init EnvManager failed";
        return err;
    }
    std::cout << "init EnvManager success";


    // 读取/创建当前文件夹的conf文件，私钥之类的信息
    // ConfManager
    std::cout << "begin to init ConfManager" << std::endl;
    err = ConfManager::instance().Init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init ConfManager failed";
        return err;
    }
    std::cout << "init ConfManager success" << std::endl;

    // 访问dbc区块链，初始化当前块高等信息
    HttpDBCChainClient::instance().init(ConfManager::instance().GetDbcChainDomain());

    // 初始化系统的各种信息（CPU,GPU等）
    // SystemInfo
    std::cout << "开始初始化 SystemInfo" << std::endl;
    SystemInfo::instance().Init(Server::NodeType, g_reserved_physical_cores_per_cpu, g_reserved_memory);
    std::cout << "初始化 SystemInfo 成功" << std::endl;

    // 初始化镜像管理信息
    // ImageManager
    std::cout << "begin to start ImageManager" << std::endl;
    ImageManager::instance().init();
    std::cout << "start ImageManager success" << std::endl;

    // timer_matrix_manager
    std::cout << "begin to init timer matrix manager" << std::endl;
    m_timer_matrix_manager = std::make_shared<timer_tick_manager>();
    err = m_timer_matrix_manager->init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init timer matrix manager failed";
        return err;
    }
    std::cout << "init timer matrix manager successful" << std::endl;

    // vxlan network manager
    std::cout << "begin to init vxlan netowrk manager" << std::endl;
    err = VxlanManager::instance().Init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init vxlan network manager failed";
        return err;
    }
    std::cout<< "init vxlan network manager successful"<<std::endl;

    // TODO: node_request_service 初始化之后，就开始了异步执行
    std::cout << "开始初始化 node_request_service" << std::endl;
    err = node_request_service::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init node_request_service failed";
        return err;
    }
    std::cout << "初始化node_request_service 成功" << std::endl;

    // network
    std::cout << "开始初始化connection manager"<< std::endl;
    err = network::connection_manager::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init connection_manager failed";
        return err;
    }
    std::cout << "初始化connection manager 成功"<< std::endl;

    // p2p_net_service
    std::cout << "开始初始化p2p_net_service" << std::endl;
    err = p2p_net_service::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "初始化p2p_net_service 失败";
        return err;
    }
    std::cout << "初始化p2p_net_service 成功"<<std::endl;

    // p2p_lan_service
    std::cout << "开始初始化p2p_lan_service" << std::endl;
    err = p2p_lan_service::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init p2p_lan_service failed";
        return err;
    }
    std::cout << "初始化p2p_lan_service 成功" << std::endl;

    std::cout << "开始初始化rest_api_service";
    err = rest_api_service::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init rest_api_service failed";
        return err;
    }
    std::cout << "初始化rest_api_service 成功" << std::endl;

    std::cout << "开始初始化http_server_service" << std::endl;
    err = http_server_service::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init http_server_service failed";
        return err;
    }
    std::cout << "初始化http_server_service 成功" << std::endl;

    std::cout << "开始初始化node_monitor_service" << std::endl;
    err = node_monitor_service::instance().init();
    if (ERR_SUCCESS != err) {
        LOG_ERROR << "init node_monitor_service failed";
        return err;
    }
    std::cout << "初始化node_monitor_service 成功" << std::endl;

    std::cout << "server init successfully" << std::endl;

    return ERR_SUCCESS;
}

ERRCODE Server::InitCrypto() {
    //openssl multithread lock
    g_ssl_lock.reset(new std::recursive_mutex[CRYPTO_num_locks()]);
    CRYPTO_set_locking_callback(ssl_locking_callback);
    OPENSSL_no_config();

    //rand seed
    RandAddSeed();                          // Seed OpenSSL PRNG with performance counter

    //elliptic curve code
    std::string sha256_algo = SHA256AutoDetect();
    RandomInit();
    ECC_Start();
    g_ecc_verify_handle.reset(new ECCVerifyHandle());

    //ecc check
    if (!ECC_InitSanityCheck())
    {
        // "Elliptic curve cryptography sanity check failure. Aborting.";
        return E_DEFAULT;
    }

    //random check
    if (!Random_SanityCheck())
    {
        //LOG_ERROR << "OS cryptographic RNG sanity check failure. Aborting.";
        return E_DEFAULT;
    }

    return ERR_SUCCESS;
}

ERRCODE Server::ExitCrypto()
{
    //rand clean
    RAND_cleanup();                         // Securely erase the memory used by the PRNG

    //openssl multithread lock
    CRYPTO_set_locking_callback(nullptr);               // Shutdown OpenSSL library multithreading support
    g_ssl_lock.reset();                         // Clear the set of locks now to maintain symmetry with the constructor.

    //ecc release
    g_ecc_verify_handle.reset();
    ECC_Stop();

    return ERR_SUCCESS;
}

// 就是初始化dbcname,nodetype变量，初始化完成后允许调用Daemon
ERRCODE Server::ParseCommandLine(int argc, char* argv[]) {
    bpo::variables_map options;
    options_description opts("command options");
    opts.add_options()
            ("version,v", "dbc version")
            ("compute", "run as compute node")
		    ("client", "run as client node")
		    ("seed", "run as seed node")
            ("name,n", bpo::value<std::string>(), "node name")
            ("daemon,d", "run as daemon process");

    try {
        bpo::store(bpo::parse_command_line(argc, argv, opts), options);
        bpo::notify(options);
    }
    catch (const std::exception &e) {
        std::cout << "parse command option error: " << e.what() << std::endl;
        std::cout << opts << std::endl;
        return ERR_ERROR;
    }

    if (options.count("version")) {
        std::cout << "version: " << dbcversion() << std::endl;
        return ERR_ERROR;
    }

    if (options.count("compute")) {
        NodeType = NODE_TYPE::ComputeNode;
    }
    else if (options.count("client")) {
        NodeType = NODE_TYPE::ClientNode;
    }
    else if (options.count("seed")) {
        NodeType = NODE_TYPE::SeedNode;
    }

    if (options.count("name")) {
        NodeName = options["name"].as<std::string>();
    }

    if (NodeName.empty()) {
        char buf[256] = { 0 };
        gethostname(buf, 256);
        NodeName = buf;
    }

	if (options.count("daemon")) {
		if (ERR_SUCCESS != Daemon())
			return ERR_ERROR;
	}

    return ERR_SUCCESS;
}

ERRCODE Server::Daemon() {
    // 将程序移到后台运行。第一个参数是0,则切换到/目录，是1布切换
    // 第二个参数是0，重定向标准输出/错误啥的到/dev/null
	if (daemon(1, 0)) {
		LOG_ERROR << "dbc daemon error: " << strerror(errno);
		return ERR_ERROR;
	}

	return ERR_SUCCESS;
}

void Server::Idle() {
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Server::Exit() {
    std::cout << "server begin exited ..."<< std::endl;

    ImageManager::instance().exit();
    std::cout << "ImageManager exited"<< std::endl;
    sleep(3);

    VxlanManager::instance().Exit();
    std::cout << "VxlanManager exited"<< std::endl;
    // exit(0);

    if (m_timer_matrix_manager) {
        m_timer_matrix_manager->exit();
    }
    std::cout << "m_timer_matrix_manager exited"<< std::endl;

    network::connection_manager::instance().exit();
    std::cout << "connection_manager exited"<< std::endl;

    p2p_lan_service::instance().exit();
    std::cout << "p2p_lan_service exited"<< std::endl;

    p2p_net_service::instance().exit();
    std::cout << "p2p_net_service exited"<< std::endl;

    http_server_service::instance().exit();
    std::cout << "http_server_service exited"<< std::endl;

    node_request_service::instance().exit();
    std::cout << "node_request_service exited"<< std::endl;

    rest_api_service::instance().exit();
    std::cout << "rest_api_service exited"<< std::endl;

    node_monitor_service::instance().exit();
    std::cout << "node_monitor_service exited"<< std::endl;

    VmClient::instance().exit();
    std::cout << "VmClient exited"<< std::endl;

    SystemInfo::instance().exit();
    std::cout << "SystemInfo exited"<< std::endl;

    ExitCrypto();
    std::cout << "Crypto exited"<< std::endl;

    m_running = false;
    std::cout << "server exited successfully"<< std::endl;
}
