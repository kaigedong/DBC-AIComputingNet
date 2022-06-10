#include "zabbixSender.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "log/log.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define ZBX_TCP_HEADER_DATA "ZBXD"

// NOTE: 用server，port初始化这个zabbix服务
// 这样有什么请求，就发给这个port就行了
zabbixSender::zabbixSender(const std::string &server, const std::string &port)
    : server_(server)
    , port_(port) {}

// NOTE: 发送一个JSONData，返回response是否成功
bool zabbixSender::sendJsonData(const std::string &json_data) {
    std::string data = "ZBXD\x01";
    unsigned long long data_len = json_data.length();
    // NOTE: 8 -> '8' ?
    char* arrLen = reinterpret_cast<char*>(&data_len);
    data.append(arrLen, 8); // NOTE: 这个char类型的长度信息，占8个字节？
    data.append(json_data);

    std::string reply;
    // NOTE: 返回的结果将会放到reply里面，其中可能包含response字段，
    // 检查一下看是否是成功
    sendData(data, reply);
    return checkResponse(reply);
}

/* NOTE: 看起来是组装了一串 json string */
bool zabbixSender::is_server_want_monitor_data(const std::string& hostname) {
    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> write(strBuf);
    write.StartObject();
    write.Key("request");
    write.String("active checks");
    write.Key("host");
    write.String(hostname.c_str());
    write.EndObject();
    std::string json_data = strBuf.GetString();
    return sendJsonData(json_data);
}

/* NOTE: Zabbix协议每个连接有1GB的数据包大小限制 */
/* Zabbix 是由 Alexei Vladishev 开发的一种网络监视、管理系统，
 * 基于 Server-Client 架构。可用于监视各种网络服务、服务器和网络机器等状态。*/
/* NOTE: 使用该方法发送请求，并获得响应！*/
/* NOTE: 发到了该类的server_, port_ 的socket里 */
void zabbixSender::sendData(const std::string &data, std::string &reply) {
    try {
        // NOTE: boost::asio库编写的代码都会用到io_service. io_service是这个库里面最重要
        // 的类，负责和操作系统打交道，等待所有异步操作的结束，然后为每一个异步操作
        // 调用其完成处理程序。
        boost::asio::io_service io_service;

        // NOTE: tcp::resolver 一般和tcp::query搭配使用，
        // 可以将server, port转为socket使用的类型
        // 而不用关心怎么实现。
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(server_, port_);

        // NOTE: 还有要说明的是, boost::asio把通讯双方(server, client)都用endpoint的表示，
        // 所以endpoint中的address, port 分别封装了ip和端口。
        // 貌似resolver和endpoint不相干，于是乎出现tcp::resolver::iterator了，
        // 它是resolver的迭代器，其实就是endpoint的指针
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
        boost::asio::ip::tcp::resolver::iterator end;

        //建立一个socket并且连接它
        boost::asio::ip::tcp::socket socket(io_service);
        boost::asio::connect(socket, iterator);

        //输出查看所有查询到的服务器
        // while (iterator != end) {
        //     boost::asio::ip::tcp::endpoint endpoint = *iterator++;
        //     std::cout << endpoint << std::endl;
        // }

        // boost::array<char, 128> buf;
        boost::system::error_code error;
        std::size_t ret;
        // socket.write_some(boost::asio::buffer(data), error);
        ret = boost::asio::write(socket, boost::asio::buffer(data), error);
        if (error) {
            LOG_ERROR << "send data error: " << error.message() << ", data size:" << data.length();
        } else {
            char buf[4096] = {0};
            // socket.read_some(boost::asio::buffer(buf), error);
            ret = boost::asio::read(socket, boost::asio::buffer(buf), error);
            if (error) {
                LOG_ERROR << "read reply error: " << error.message();
            }
            unsigned long long jsonLength = 0;
            const char* jsonstr = getJsonString(buf, jsonLength);
            if (jsonstr && jsonLength > 0 && ret > jsonLength) {
                reply.resize(jsonLength);
                memcpy(&reply[0], jsonstr, jsonLength);
            } else {
                LOG_ERROR << "parse zabbix reply error";
                // LOG_ERROR << "parse zabbix reply error(string begin: " << jsonstr << ", string length: " << jsonLength << ")";
            }
        }
    } catch (std::exception &e) {
        LOG_ERROR << "Exception: " << e.what();
    }
}

/* NOTE: 检查response == success */
bool zabbixSender::checkResponse(const std::string &reply) {
    rapidjson::Document doc;
    doc.Parse(reply.c_str());
    if (!doc.IsObject()) return false;
    if (!doc.HasMember("response")) return false;
    if (!doc["response"].IsString()) return false;
    std::string response = doc["response"].GetString();
    return response == "success";
}

/* NOTE: 获得result的长度
 * ZBXD
 * 标头存在于 Zabbix 组件之间的响应和请求消息中。
 * 需要确定消息的长度，是否压缩以及消息长度、字段的格式。标头包含: */
const char* zabbixSender::getJsonString(const char* result, unsigned long long &jsonLength) {
    // strstr: 找到第一次出现的位置地址(返回char*类型)
    const char* zbx_tcp_header = strstr(result, ZBX_TCP_HEADER_DATA);
    if (!zbx_tcp_header) return nullptr;
    /* reinterpret_cast:强制类型转换操作 */
    jsonLength = *reinterpret_cast<const unsigned long long*>(zbx_tcp_header + 5);
    return zbx_tcp_header + 5 + 8;
}
