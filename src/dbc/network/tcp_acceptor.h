#ifndef DBC_NETWORK_TCP_ACCEPTOR_H
#define DBC_NETWORK_TCP_ACCEPTOR_H

#include <memory>
#include <boost/asio.hpp>
#include "nio_loop_group.h"
#include "channel.h"
#include "log/log.h"
#include "handler_create_functor.h"

using namespace boost::asio::ip;

#define DEFAULT_LISTEN_BACKLOG                          8                               //default listen backlog

namespace dbc
{
    namespace network
    {
        class tcp_acceptor : public std::enable_shared_from_this<tcp_acceptor>, boost::noncopyable
        {
            //using ios_ptr = typename std::shared_ptr<io_service>;
            using ios_weak_ptr = typename std::weak_ptr<io_service>;
            using nio_loop_ptr = typename std::shared_ptr<nio_loop_group>;

        public:
            //tcp_acceptor(ios_ptr io_service, nio_loop_ptr worker_group, tcp::endpoint endpoint, handler_create_functor func);
            tcp_acceptor(ios_weak_ptr io_service, nio_loop_ptr worker_group, tcp::endpoint endpoint, handler_create_functor func);

            virtual ~tcp_acceptor() = default;

            virtual int32_t start();

            virtual int32_t stop();

            ip::tcp::endpoint get_endpoint() const { return m_endpoint; }

        protected:

            virtual int32_t create_channel();

            virtual int32_t on_accept(std::shared_ptr<channel> ch, const boost::system::error_code& error);

        protected:

            ip::tcp::endpoint m_endpoint;

            //ios_ptr m_io_service;
            ios_weak_ptr m_io_service;

            nio_loop_ptr m_worker_group;

            tcp::acceptor m_acceptor;

            handler_create_functor m_handler_create_func;

        };
    }
}

#endif