#include "io_service_pool.h"

namespace network
{
    io_service_pool::io_service_pool() {}

    io_service_pool::~io_service_pool() {
        // 在 C++ 中，每一个对象都能通过 this 指针来访问自己的地址。
        // this 指针是所有成员函数的隐含参数。
        // 因此，在成员函数内部，它可以用来指向调用对象。
        this->exit();
    }

    // REVIEW: 当有异步任务时，io_service::run会一直阻塞，没有任务了，run会返回，所有异步操作终止
    // 下面这种使用io_service::work(io_service_)，即使没有任务run也不会返回
    ERRCODE io_service_pool::init(size_t thread_size) {
        // REVIEW: 将处理异步的线程初始化为1个
        m_thread_size = thread_size;
        for (size_t i = 0; i < thread_size; i++) {
            std::shared_ptr<boost::asio::io_service> ioservice = std::make_shared<boost::asio::io_service>();
            m_io_services.push_back(ioservice);
            std::shared_ptr<boost::asio::io_service::work> iowork = std::make_shared<boost::asio::io_service::work>(*ioservice.get());
            m_works.push_back(iowork);
        }

        return ERR_SUCCESS;
    }

    // 调用每个线程的run方法
    ERRCODE io_service_pool::start() {
        if (!m_running) {
            m_running = true;

            // REVIEW: m_thread_size = 1
            // 下面将运行第0个任务
            // i = 1没有意义，因为需要等待i = 0的完成
            for (size_t i = 0; i < m_thread_size; i++) {
                std::shared_ptr<std::thread> thr(new std::thread(boost::bind(&boost::asio::io_service::run, m_io_services[i])));
                m_threads.push_back(thr);
            }
        }

        return ERR_SUCCESS;
    }

    // 调用每个线程的stop方法
    ERRCODE io_service_pool::stop() {
        if (m_running) {
            m_running = false;

            for (size_t i = 0; i < m_thread_size; i++)
            {
                m_io_services[i]->stop();
            }

            for (size_t i = 0; i < m_thread_size; i++) {
                if (m_threads[i]->joinable()) {
                    m_threads[i]->join();
                }
            }
            m_threads.clear();
        }

        return ERR_SUCCESS;
    }

    ERRCODE io_service_pool::exit() {
        this->stop();
        m_works.clear();
        m_io_services.clear();

        return ERR_SUCCESS;
    }

    // 返回下一个线程以供使用
    std::shared_ptr<boost::asio::io_service> io_service_pool::get_io_service() {
        std::unique_lock<std::mutex> lock(m_mutex);
        size_t idx = (m_cur_io_service + 1) % m_thread_size;
        return m_io_services[idx];
    }
}
