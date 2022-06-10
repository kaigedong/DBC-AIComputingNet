#ifndef PEER_CANDIDATE_H
#define PEER_CANDIDATE_H

#include "util/utils.h"
#include "peer_node.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "util/crypto/utilstrencodings.h"
#include <boost/asio.hpp>

namespace bfs = boost::filesystem;
using namespace boost::asio;

// 啊？这个变量，只出现过一次
#define DEFAULT_CONNECT_PEER_NODE      102400       //default connect peer nodes
#define DAT_PEERS_FILE_NAME            "peers.dat"

// TODO: 定义node的网络状态
enum net_state
{
    // idle：英文，闲置的
    ns_idle = 0,   // can use whenever needed
    ns_in_use,     // connecting or connected
    ns_failed,     // not use within a long time
    ns_zombie,
    ns_available
};

// NOTE: 根据net_state将状态转为string
// 相当于实现了json化
static std::string net_state_2_string(int8_t st)
{
    switch ((net_state)st)
    {
        case ns_idle:
            return "idle";
        case ns_in_use:
            return "in_use";
        case ns_failed:
            return "failed";
        case ns_zombie:
            return "zombie";
        case ns_available:
            return "available";
        default:
            return "";
    }
}

struct peer_candidate
{
    ip::tcp::endpoint   tcp_ep; // NOTE: TCP的接口，包括IP，Port应该是
    peer_node_type  node_type = PEER_NORMAL_NODE; // 默认为Normal
    std::string     node_id; // 每个Peer必须要有node_id
    net_state       net_st = ns_idle; // peer的网络状态
    uint32_t        reconn_cnt = 0;
    time_t          last_conn_tm; // 上次连接时间？
    uint32_t        score = 0;  //indicate level of Qos, update when disconnect

    // struct中定义的函数，默认为public，class中定义的函数则默认为private
    peer_candidate() {
        last_conn_tm = time(nullptr);
    }

    // NOTE: 定义一个初始化的函数，用这些参数初始化该结构体
    peer_candidate(ip::tcp::endpoint ep, net_state _net_state = ns_idle,
        peer_node_type _peer_node_type = PEER_NORMAL_NODE, uint32_t _reconn_cnt = 0, 
        time_t _last_conn_tm = time(nullptr), uint32_t _score = 0, std::string _node_id = "")
        : tcp_ep(ep)
        , net_st(_net_state)
        , reconn_cnt(_reconn_cnt)
        , last_conn_tm(_last_conn_tm)
        , score(_score)
        , node_id(_node_id)
        , node_type(_peer_node_type)
    {

    }
};

#endif
