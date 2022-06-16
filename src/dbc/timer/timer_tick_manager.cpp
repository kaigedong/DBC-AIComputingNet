#include "timer_tick_manager.h"
#include "time_tick_notification.h"
#include "network/topic_manager.h"

std::atomic<uint64_t> timer_tick_manager::m_cur_tick = {0};

ERRCODE timer_tick_manager::init()
{
    m_timer_group = std::make_shared<network::io_service_pool>();
    ERRCODE err = m_timer_group->init(1);
    if (ERR_SUCCESS != err) {
        return err;
    }

    err = m_timer_group->start();
	if (ERR_SUCCESS != err) {
		return err;
	}

	m_timer = std::make_shared<steady_timer>(*(m_timer_group->get_io_service()));
    // 100ms过期
	m_timer->expires_from_now(std::chrono::milliseconds(DEFAULT_TIMER_INTERVAL));

    // 1. async_wait形参只有一个所以需要bind绑定函数和函数参数
    // bind 是一组重载的函数模板.用来向一个函数(或函数对象)绑定某些参数.
    // 2. 对于计时器，用异步的方式，到相对于现在100ms的时候，再执行
	m_timer->async_wait(boost::bind(&timer_tick_manager::on_timer_expired, shared_from_this(), boost::asio::placeholders::error));

    return ERR_SUCCESS;
}

void timer_tick_manager::exit() {
	boost::system::error_code error;
	m_timer->cancel(error);
    m_timer_group->stop();
    m_timer_group->exit();
}

void timer_tick_manager::on_timer_expired(const boost::system::error_code& error)
{
    if (boost::asio::error::operation_aborted == error) {
        return;
    }

    ++m_cur_tick;

    //publish notification
    std::shared_ptr<network::message> msg = make_time_tick_notification();
    std::cout << "在timer_tick_manage.on_timer_expired中调用publish" << std::endl;
    std::cout << "在timer_tick_manage.on_timer_expired发布" << msg->get_name() << std::endl;
    topic_manager::instance().publish<void>(msg->get_name(), msg);

	m_timer->expires_from_now(std::chrono::milliseconds(DEFAULT_TIMER_INTERVAL));

    // 异步阻塞，到100ms后将会执行boost::bind中的函数，即再执行一遍on_timer_expired，
	m_timer->async_wait(boost::bind(&timer_tick_manager::on_timer_expired, shared_from_this(), boost::asio::placeholders::error));
}

std::shared_ptr<network::message> timer_tick_manager::make_time_tick_notification()
{
    std::shared_ptr<time_tick_notification> content(new time_tick_notification);
    content->time_tick = m_cur_tick;

    std::shared_ptr<network::message> msg = std::make_shared<network::message>();
    msg->set_name(TIMER_TICK_NOTIFICATION);
    msg->set_content(content);

    return msg;
}
