﻿#include "http_server_service.h"
#include <assert.h>
#include <future>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/http.h>
#include "server/server.h"
#include "log/log.h"
#include "network/http_client.h"
#include "rest_handler.h"
#include "rest_api_service.h"

int32_t http_server_service::init(bpo::variables_map &options) {
    int32_t ret = load_rest_config(options);
    if (ret != E_SUCCESS) {
        LOG_ERROR << "http server service load config error";
        return ret;
    }

    rest_api_service::instance();

    if (m_listen_port == 0) {
        return E_SUCCESS;
    }

    if (!init_http_server()) {
        return E_EXIT_FAILURE;
    }

    return E_SUCCESS;
}

int32_t http_server_service::load_rest_config(bpo::variables_map &options) {
    std::string conf_rest_ip = options.count("rest_ip") ? options["rest_ip"].as<std::string>()
                                                        : conf_manager::instance().get_rest_ip();
    ip_validator ip_vdr;
    variable_value val;
    val.value() = conf_rest_ip;
    if (!ip_vdr.validate(val)) {
        LOG_ERROR << "http server init invalid ip: " << conf_rest_ip;
        return E_DEFAULT;
    }
    m_listen_ip = conf_rest_ip;

    // rest port
    std::string conf_rest_port = options.count("rest_port") ? options["rest_port"].as<std::string>()
            : conf_manager::instance().get_rest_port();
    port_validator port_vdr;
    val.value() = conf_rest_port;
    if ((conf_rest_port != "0") && (port_vdr.validate(val) == false)) {
        LOG_ERROR << "http server init invalid port: " << conf_rest_port;
        return E_DEFAULT;
    } else {
        try {
            m_listen_port = (uint16_t) std::stoi(conf_rest_port);
        } catch (const std::exception &e) {
            LOG_ERROR << "http server init transform exception. invalid port: " << conf_rest_port << ", "
                      << e.what();
            return E_DEFAULT;
        }
    }

    LOG_INFO << "rest config: " << "rest ip:" << m_listen_ip << ", rest port:" << m_listen_port;
    return E_SUCCESS;
}

bool http_server_service::init_http_server() {
    // todo : support acl list in futurre
    raii_event_base base_ctr = obtain_event_base();

    raii_evhttp http_ctr = obtain_evhttp(base_ctr.get());
    struct evhttp *http = http_ctr.get();
    if (!http) {
        LOG_ERROR << "couldn't create evhttp. Exiting.";
        return false;
    }
    evhttp_set_timeout(http, DEFAULT_HTTP_SERVER_TIMEOUT);
    evhttp_set_max_headers_size(http, MAX_HEADERS_SIZE);
    evhttp_set_max_body_size(http, MAX_BODY_SIZE);
    evhttp_set_gencb(http, http_server_service::http_request_cb, this);

    if (!http_bind_addresses(http)) {
        LOG_ERROR << "Unable to bind any endpoint for RPC server";
        return false;
    }

    m_event_base = base_ctr.release();
    m_event_http = http_ctr.release();
    return true;
}

bool http_server_service::http_bind_addresses(struct evhttp *http) {
    evhttp_bound_socket *bind_handle = evhttp_bind_socket_with_handle(http, m_listen_ip.c_str(), m_listen_port);
    if (bind_handle) {
        m_bound_sockets.push_back(bind_handle);
    } else {
        LOG_ERROR << "Binding RPC failed on address： " << m_listen_ip << " port: " << m_listen_port;
    }

    return !m_bound_sockets.empty();
}

void http_server_service::http_request_cb(struct evhttp_request *req, void *arg) {
    // Disable reading to work around a libevent bug, fixed in 2.2.0.
    if (event_get_version_number() >= 0x02010600 && event_get_version_number() < 0x02020001) {
        evhttp_connection *conn = evhttp_request_get_connection(req);
        if (conn) {
            bufferevent *bev = evhttp_connection_get_bufferevent(conn);
            if (bev) {
                bufferevent_disable(bev, EV_READ);
            }
        }
    }

    assert(arg);
    http_server_service *pthis = reinterpret_cast<http_server_service *>(arg);
    pthis->on_http_request_event(req);

}

void http_server_service::on_http_request_event(struct evhttp_request *req) {
    std::shared_ptr<dbc::network::http_request> hreq(new dbc::network::http_request(req, m_event_base));

    LOG_DEBUG << "Received a " << hreq->request_method_string(hreq->get_request_method()) << ", request for "
              << hreq->get_uri() << " from " << hreq->get_peer().get_ip() << std::endl;

    rest_api_service::instance().on_http_request_event(hreq);
}

void http_server_service::start_http_server() {
    std::packaged_task<bool(event_base *, evhttp *)> task(thread_http_fun);
    m_thread_result = task.get_future();
    m_thread_http = std::thread(std::move(task), m_event_base, m_event_http);
}

bool http_server_service::thread_http_fun(struct event_base *base, struct evhttp *http) {
    rename_thread("dbc-http");
    LOG_DEBUG << "Entering http event loop";
    event_base_dispatch(base);
    // Event loop will be interrupted by InterruptHTTPServer()
    LOG_DEBUG << "Exited http event loop";
    return event_base_got_break(base) == 0;
}

void http_server_service::interrupt_http_server() {
    if (m_event_http) {
        for (evhttp_bound_socket *socket : m_bound_sockets) {
            evhttp_del_accept_socket(m_event_http, socket);
        }

        evhttp_set_gencb(m_event_http, http_reject_request_cb, nullptr);
    }
}

void http_server_service::http_reject_request_cb(struct evhttp_request *req, void *) {
    LOG_INFO << "Rejecting request while shutting down";
    evhttp_send_error(req, HTTP_SERVUNAVAIL, nullptr);
}

void http_server_service::stop_http_server() {
    if (m_event_base) {
        LOG_DEBUG << "Waiting for HTTP event thread to exit";
        // Exit the event loop as soon as there are no active events.
        event_base_loopexit(m_event_base, nullptr);
        // Give event loop a few seconds to exit (to send back last RPC responses), then break it
        // Before this was solved with event_base_loopexit, but that didn't work as expected in
        // at least libevent 2.0.21 and always introduced a delay. In libevent
        // master that appears to be solved, so in the future that solution
        // could be used again (if desirable).
        if (m_thread_result.valid()
            && m_thread_result.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout) {
            LOG_DEBUG << "HTTP event loop did not exit within allotted time, sending loopbreak";
            event_base_loopbreak(m_event_base);
        }
        m_thread_http.join();
    }

    if (m_event_http) {
        evhttp_free(m_event_http);
        m_event_http = nullptr;
    }

    if (m_event_base) {
        event_base_free(m_event_base);
        m_event_base = nullptr;
    }
}

void http_server_service::rename_thread(const char *name) {
#if defined(PR_SET_NAME)
    // Only the first 15 characters are used (16 - NUL terminator)
    ::prctl(PR_SET_NAME, name, 0, 0, 0);
#elif defined(MAC_OSX)
    pthread_setname_np(name);
#else
    // Prevent warnings for unused parameters...
    (void) name;
#endif
}