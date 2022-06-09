#ifndef DBC_PEER_NODE_H
#define DBC_PEER_NODE_H

#include "util/utils.h"
#include "network/utils/net_address.h"
#include "network/utils/socket_id.h"
#include "message/matrix_types.h"

enum peer_node_type
{
    PEER_NORMAL_NODE = 0, //config peer ( peer.conf )
    PEER_SEED_NODE = 1  //seed node
};

enum connection_status
{
    DISCONNECTED = 0,
    CONNECTED
};

class peer_node
{
    // 声明一个友员类，这个类可以操作/访问本类的私有/保护成员
    // 我寻思，这也没private/protect啊
    friend class p2p_net_service;

    // 同上
    friend void assign_peer_info(dbc::peer_node_info& info, const std::shared_ptr<peer_node>& node);

public:
    peer_node() = default;

    virtual ~peer_node() = default;

    peer_node(const peer_node& other);

    peer_node& operator=(const peer_node& other);

public:
    // 一个enum类型，用来表示该节点是种子节点还是普通节点
    peer_node_type m_node_type = PEER_NORMAL_NODE;
    std::string m_id;

    network::socket_id m_sid;

    int32_t m_core_version = 0;
    int32_t m_protocol_version = 0;

    int64_t m_connected_time = 0;
    int64_t m_live_time = 0;

    // enum类型，表示连接状态是0/1
    connection_status m_connection_status = DISCONNECTED;

    network::net_address m_peer_addr;
    network::net_address m_local_addr;
};

// 表示这个实现是在外部
extern void assign_peer_info(dbc::peer_node_info& info, const std::shared_ptr<peer_node>& node);

#endif
