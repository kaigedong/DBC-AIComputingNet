#ifndef DBC_MULTICAST_SENDER_H
#define DBC_MULTICAST_SENDER_H

#include <string>
#include <boost/asio.hpp>

// 多播
class multicast_sender {
public:
    // 编译器会默认给类生成默认函数，如果不想使用其中某个，可以将其定义为
    // private或者=delete;
    // 下面相当于不使用默认构造函数
    multicast_sender() = delete;
    multicast_sender(boost::asio::io_context& io_context,
        const boost::asio::ip::address& multicast_address,
        short multicast_port);
    ~multicast_sender() = default;
    
    void send(const std::string& message);
    
protected:
private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
    std::string message_;
};

#endif
