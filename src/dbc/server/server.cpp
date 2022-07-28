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

// REVIEW: NOTE_TYPE 有三种： ComputeNode, ClientNode, SeedNode
NODE_TYPE Server::NodeType = NODE_TYPE::ComputeNode;
std::string Server::NodeName = "";

// // main函数中的第一步，初始化
// REVIEW: Init函数中下面删除了多个类似下面的错误处理：
//
// if (ERR_SUCCESS != err) {
//     LOG_ERROR << "init node_monitor_service failed";
//     return err;
// }
ERRCODE Server::Init(int argc, char *argv[]) {
    ERRCODE err = ERR_SUCCESS;

    err = ParseCommandLine(argc, argv); // 初始化参数（NodeType, Nodename), 及是否移到后台
    err = dbclog::instance().init(); // (略)初始化log设置
    std::cout << "开始server初始化" << std::endl;
    err = InitCrypto(); // (略)初始化openssl随机数
    err = EnvManager::instance().Init(); // 初始化打小端，位置，配置文件位置
    err = ConfManager::instance().Init(); // 读取/创建当前文件夹的conf文件，私钥之类的信息
    // 访问dbc区块链，初始化选择一个链HTTP服务
    HttpDBCChainClient::instance().init(ConfManager::instance().GetDbcChainDomain());

    // 初始化系统的各种信息（公网IP，CPU,GPU等）
    // 开启一个线程，持续更新各种信息
    SystemInfo::instance().Init(Server::NodeType, g_reserved_physical_cores_per_cpu, g_reserved_memory);
    // 实际上是开启一个线程，循环的检查是否有正在下载/上传的镜像
    // 并添加了上传/下载完成后的hook函数
    ImageManager::instance().init(); // 初始化镜像管理信息

    m_timer_matrix_manager = std::make_shared<timer_tick_manager>();
    err = m_timer_matrix_manager->init();
    err = VxlanManager::instance().Init();
    err = node_request_service::instance().init(); // TODO: node_request_service 初始化之后，就开始了异步执行
    err = network::connection_manager::instance().init(); // TODO: 这里初始化了什么？
    err = p2p_net_service::instance().init();
    err = p2p_lan_service::instance().init();
    err = rest_api_service::instance().init();
    err = http_server_service::instance().init();
    err = node_monitor_service::instance().init();

    std::cout << "server 初始化成功" << std::endl;
    return ERR_SUCCESS;
}

// REVIEW: 初始化SSL的一些随机数，似乎与HTTPS加密有关
static std::unique_ptr<ECCVerifyHandle> g_ecc_verify_handle;
static std::unique_ptr<std::recursive_mutex[]> g_ssl_lock;

void ssl_locking_callback(int mode, int type, const char *file, int line){
    if (mode & CRYPTO_LOCK) {
        g_ssl_lock[type].lock();
    } else {
        g_ssl_lock[type].unlock();
    }
}

ERRCODE Server::InitCrypto() {
    // openssl multithread lock
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

ERRCODE Server::ExitCrypto(){
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

// REVIEW: 根据参数, 初始化Server::NodeName和Server::NodeType两个值
ERRCODE Server::ParseCommandLine(int argc, char* argv[]) {
    bpo::variables_map options;
    options_description opts("command options");
    opts.add_options()
            // ("version,v", "dbc version")
            ("compute", "run as compute node")
            ("client", "run as client node")
            ("seed", "run as seed node")
            ("name,n", bpo::value<std::string>(), "node name");
            // ("daemon,d", "run as daemon process");

    // REVIEW: 如果解析失败，则退出
    try {
        bpo::store(bpo::parse_command_line(argc, argv, opts), options);
        bpo::notify(options);
    } catch (const std::exception &e) {
        std::cout << "parse command option error: " << e.what() << std::endl;
        std::cout << opts << std::endl;
        return ERR_ERROR;
    }

    // 返回dbc version
    // if (options.count("version")) {
    //     std::cout << "version: " << dbcversion() << std::endl;
    //     return ERR_ERROR;
    // }

    // 根据参数，决定NodeType是那种类型
    if (options.count("compute")) {
        NodeType = NODE_TYPE::ComputeNode;
    } else if (options.count("client")) {
        NodeType = NODE_TYPE::ClientNode;
    } else if (options.count("seed")) {
        NodeType = NODE_TYPE::SeedNode;
    }

    if (options.count("name")) {
        NodeName = options["name"].as<std::string>();
    }
    // REVIEW: 如果为空，则获取hostname并赋值
    // if (NodeName.empty()) {
    //     char buf[256] = { 0 };
    //     gethostname(buf, 256);
    //     NodeName = buf;
    // }
    // REVIEW: 以后台运行
    // if (options.count("daemon")) {
    //     Daemon();
    // }

    return ERR_SUCCESS;
}

// REVIEW: 将程序移到后台运行。第一个参数是0,则切换到/目录，是1布切换
// 第二个参数是0，重定向标准输出/错误啥的到/dev/null
// ERRCODE Server::Daemon() {
//     if (daemon(1, 0)) {
//         LOG_ERROR << "dbc daemon error: " << strerror(errno);
//         return ERR_ERROR;
//     }
//     return ERR_SUCCESS;
// }

// REVIEW: 如果在运行，则休眠，否则返回后将结束主线程
void Server::Idle() {
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// 释放资源
// void Server::Exit() {
//     std::cout << "server begin exited ..."<< std::endl;
//     ImageManager::instance().exit();
//     std::cout << "ImageManager exited"<< std::endl;
//     sleep(3);
//     VxlanManager::instance().Exit();
//     std::cout << "VxlanManager exited"<< std::endl;
//     // exit(0);
//     if (m_timer_matrix_manager) {
//         m_timer_matrix_manager->exit();
//     }
//     std::cout << "m_timer_matrix_manager exited"<< std::endl;
//     network::connection_manager::instance().exit();
//     std::cout << "connection_manager exited"<< std::endl;
//     p2p_lan_service::instance().exit();
//     std::cout << "p2p_lan_service exited"<< std::endl;
//     p2p_net_service::instance().exit();
//     std::cout << "p2p_net_service exited"<< std::endl;
//     http_server_service::instance().exit();
//     std::cout << "http_server_service exited"<< std::endl;
//     node_request_service::instance().exit();
//     std::cout << "node_request_service exited"<< std::endl;
//     rest_api_service::instance().exit();
//     std::cout << "rest_api_service exited"<< std::endl;
//     node_monitor_service::instance().exit();
//     std::cout << "node_monitor_service exited"<< std::endl;
//     VmClient::instance().exit();
//     std::cout << "VmClient exited"<< std::endl;
//     SystemInfo::instance().exit();
//     std::cout << "SystemInfo exited"<< std::endl;
//     ExitCrypto();
//     std::cout << "Crypto exited"<< std::endl;
//     m_running = false;
//     std::cout << "server exited successfully"<< std::endl;
// }
