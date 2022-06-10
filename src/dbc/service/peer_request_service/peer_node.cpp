#include "peer_node.h"

// NOTE: peer_node的有参构造函数
// 类peer_node下的peer_node方法的定义：
// 即：从一个peer_node(other)生成一个peer_node
peer_node::peer_node(const peer_node& other)
{
    this->m_node_type = other.m_node_type;
    this->m_id = other.m_id;
    this->m_sid = other.m_sid;
    this->m_core_version = other.m_core_version;
    this->m_protocol_version = other.m_protocol_version;
    this->m_connected_time = other.m_connected_time;
    this->m_live_time = other.m_live_time;
    this->m_connection_status = other.m_connection_status;
    this->m_peer_addr = other.m_peer_addr;
    this->m_local_addr = other.m_local_addr;
}

// NOTE: 用operator关键字，重载了=操作符
// 如果不相等，那就赋值一下。
peer_node& peer_node::operator=(const peer_node& other)
{
    if (this != &other)
    {
        this->m_node_type = other.m_node_type;
        this->m_id = other.m_id;
        this->m_sid = other.m_sid;
        this->m_core_version = other.m_core_version;
        this->m_protocol_version = other.m_protocol_version;
        this->m_connected_time = other.m_connected_time;
        this->m_live_time = other.m_live_time;
        this->m_connection_status = other.m_connection_status;
        this->m_peer_addr = other.m_peer_addr;
        this->m_local_addr = other.m_local_addr;
    }

    return *this;
}

// NOTE: 将peer_node类的信息给peer_node_info类
void assign_peer_info(dbc::peer_node_info& info, const std::shared_ptr<peer_node>& node)
{
    info.peer_node_id = node->m_id;
    info.core_version = node->m_core_version;
    info.protocol_version = node->m_protocol_version;
    info.live_time_stamp = (int32_t)node->m_live_time;
    info.addr.ip = node->m_peer_addr.get_ip();
    info.addr.port = node->m_peer_addr.get_port();
}
