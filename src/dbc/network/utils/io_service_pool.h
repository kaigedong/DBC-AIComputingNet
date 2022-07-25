#ifndef DBC_NETWORK_NIO_LOOP_GROUP_H
#define DBC_NETWORK_NIO_LOOP_GROUP_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "common/common.h"

namespace network
{
    class io_service_pool
    {
    public:
        io_service_pool();

        virtual ~io_service_pool();

        ERRCODE init(size_t thread_size);

        ERRCODE start();

        ERRCODE stop();

        ERRCODE exit();

        std::shared_ptr<boost::asio::io_service> get_io_service();

    protected:
        std::mutex m_mutex;
        // 当前使用的线程的index，用来分配
        size_t m_cur_io_service = 0;
        // Boost.Asio 有两种支持多线程的方式，
        //
        // 第一种方式比较简单：
        // 在多线程的场景下，每个线程都持有一个io_service
        // 并且每个线程都调用各自的io_service的run()方法。
        // 根据线程数量，创建与线程数相同的io_service和work
        //
        // 另一种支持多线程的方式：
        // 全局只分配一个io_service，并且让这个io_service在多个线程之间共享，
        // 每个线程都调用全局的io_service的run()方法。
        std::vector<std::shared_ptr<boost::asio::io_service>> m_io_services;
        std::vector<std::shared_ptr<boost::asio::io_service::work>> m_works;

        size_t m_thread_size = 1; // 线程数
        std::vector<std::shared_ptr<std::thread>> m_threads;
        bool m_running = false;
    };
}

#endif
