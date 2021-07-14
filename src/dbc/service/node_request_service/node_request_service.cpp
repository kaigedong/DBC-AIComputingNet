#include <boost/property_tree/json_parser.hpp>
#include <cassert>
#include <boost/exception/all.hpp>
#include "server.h"
#include "conf_manager.h"
#include "node_request_service.h"
#include "service_message_id.h"
#include "matrix_types.h"
#include "matrix_coder.h"
#include "ip_validator.h"
#include "port_validator.h"
#include "task_common_def.h"
#include "util.h"
#include <boost/thread/thread.hpp>
#include "service_topic.h"
#include <ctime>
#include <boost/format.hpp>
#include "url_validator.h"
#include "time_util.h"
#include <stdlib.h>
#include <boost/algorithm/string/join.hpp>
#include "message_id.h"
#include "comm.h"
#include "json_util.h"

using namespace matrix::core;
using namespace matrix::service_core;
using namespace ai::dbc;

namespace dbc {
	std::string get_gpu_spec(const std::string& s) {
		if (s.empty()) {
			return "";
		}

		std::string rt;
		std::stringstream ss;
		ss << s;
		boost::property_tree::ptree pt;
		try {
			boost::property_tree::read_json(ss, pt);
			rt = pt.get<std::string>("env.NVIDIA_VISIBLE_DEVICES");
			if (!rt.empty()) {
				matrix::core::string_util::trim(rt);
			}
		}
		catch (...) {

		}

		return rt;
	}

	std::string get_is_update(const std::string& s) {
		if (s.empty()) return "";

		std::string operation;
		std::stringstream ss;
		ss << s;
		boost::property_tree::ptree pt;

		try {
			boost::property_tree::read_json(ss, pt);
			if (pt.count("operation") != 0) {
				operation = pt.get<std::string>("operation");
			}
		}
		catch (...) {

		}

		return operation;
	}

	node_request_service::node_request_service() {

	}

	void node_request_service::init_timer() {
		// 配置文件：timer_ai_training_task_schedule_in_second（15秒）
		m_timer_invokers[AI_TRAINING_TASK_TIMER] = std::bind(&node_request_service::on_training_task_timer,
			this, std::placeholders::_1);
		m_training_task_timer_id = this->add_timer(AI_TRAINING_TASK_TIMER,
			CONF_MANAGER->get_timer_ai_training_task_schedule_in_second() *
			1000, ULLONG_MAX, DEFAULT_STRING);

		// 10分钟
		m_timer_invokers[AI_PRUNE_TASK_TIMER] = std::bind(&node_request_service::on_prune_task_timer, this,
			std::placeholders::_1);
		m_prune_task_timer_id = this->add_timer(AI_PRUNE_TASK_TIMER, AI_PRUNE_TASK_TIMER_INTERVAL, ULLONG_MAX,
			DEFAULT_STRING);

		assert(INVALID_TIMER_ID != m_training_task_timer_id);
		assert(INVALID_TIMER_ID != m_prune_task_timer_id);
	}

	void node_request_service::init_invoker() {
		invoker_type invoker;
		BIND_MESSAGE_INVOKER(NODE_CREATE_TASK_REQ, &node_request_service::on_node_create_task_req);
		BIND_MESSAGE_INVOKER(NODE_START_TASK_REQ, &node_request_service::on_node_start_task_req);
		BIND_MESSAGE_INVOKER(NODE_RESTART_TASK_REQ, &node_request_service::on_node_restart_task_req);
		BIND_MESSAGE_INVOKER(NODE_STOP_TASK_REQ, &node_request_service::on_node_stop_task_req);
        BIND_MESSAGE_INVOKER(NODE_RESET_TASK_REQ, &node_request_service::on_node_reset_task_req);
        BIND_MESSAGE_INVOKER(NODE_DELETE_TASK_REQ, &node_request_service::on_node_delete_task_req);
        BIND_MESSAGE_INVOKER(NODE_TASK_LOGS_REQ, &node_request_service::on_node_task_logs_req);
		BIND_MESSAGE_INVOKER(NODE_LIST_TASK_REQ, &node_request_service::on_node_list_task_req);
		// node_info_collection 定期广播
		BIND_MESSAGE_INVOKER(typeid(get_task_queue_size_req_msg).name(), &node_request_service::on_get_task_queue_size_req);
	}

	void node_request_service::init_subscription() {
		SUBSCRIBE_BUS_MESSAGE(NODE_CREATE_TASK_REQ);
		SUBSCRIBE_BUS_MESSAGE(NODE_START_TASK_REQ);
		SUBSCRIBE_BUS_MESSAGE(NODE_RESTART_TASK_REQ);
		SUBSCRIBE_BUS_MESSAGE(NODE_STOP_TASK_REQ);
        SUBSCRIBE_BUS_MESSAGE(NODE_RESET_TASK_REQ);
        SUBSCRIBE_BUS_MESSAGE(NODE_DELETE_TASK_REQ);
        SUBSCRIBE_BUS_MESSAGE(NODE_TASK_LOGS_REQ);
		SUBSCRIBE_BUS_MESSAGE(NODE_LIST_TASK_REQ);

		//task queue size query
		SUBSCRIBE_BUS_MESSAGE(typeid(get_task_queue_size_req_msg).name());
	}

	int32_t node_request_service::service_init(bpo::variables_map& options) {
	    auto fresult = m_task_scheduler.Init();
	    int32_t ret = std::get<0>(fresult);

		if (ret != E_SUCCESS) {
			return E_DEFAULT;
		}
		else {
			return E_SUCCESS;
		}
	}

	int32_t node_request_service::service_exit() {
		remove_timer(m_training_task_timer_id);
		remove_timer(m_prune_task_timer_id);
		return E_SUCCESS;
	}

	bool node_request_service::hit_node(const std::vector<std::string>& peer_node_list, const std::string& node_id) {
		bool hit = false;
		auto it = peer_node_list.begin();
		for (; it != peer_node_list.end(); it++) {
			if ((*it) == node_id) {
				hit = true;
				break;
			}
		}

		return hit;
	}

	bool node_request_service::check_nonce(const std::string& nonce) {
		if (!dbc::check_id(nonce)) {
			return false;
		}

		if (m_nonceCache.contains(nonce)) {
			return false;
		}
		else {
			m_nonceCache.insert(nonce, 1);
		}

		return true;
	}

    bool node_request_service::check_req_header(std::shared_ptr<dbc::network::message> &msg) {
        if (!msg) {
            LOG_ERROR << "msg is nullptr";
            return false;
        }

        std::shared_ptr<dbc::network::msg_base> base = msg->content;
        if (!base) {
            LOG_ERROR << "msg.containt is nullptr";
            return false;
        }

        if (!dbc::check_id(base->header.nonce)) {
            LOG_ERROR << "header.nonce check failed";
            return false;
        }

        if (!dbc::check_id(base->header.session_id)) {
            LOG_ERROR << "header.session_id check failed";
            return false;
        }

        if (base->header.path.size() <= 0) {
            LOG_ERROR << "header.path size <= 0";
            return false;
        }

        if (base->header.exten_info.size() < 4) {
            LOG_ERROR << "header.exten_info size < 4";
            return E_DEFAULT;
        }

        return true;
    }

    bool node_request_service::check_rsp_header(std::shared_ptr<dbc::network::message> &msg) {
        if (!msg) {
            LOG_ERROR << "msg is nullptr";
            return false;
        }

        std::shared_ptr<dbc::network::msg_base> base = msg->content;
        if (!base) {
            LOG_ERROR << "msg.containt is nullptr";
            return false;
        }

        if (!dbc::check_id(base->header.nonce)) {
            LOG_ERROR << "header.nonce check failed";
            return false;
        }

        if (!dbc::check_id(base->header.session_id)) {
            LOG_ERROR << "header.session_id check failed";
            return false;
        }

        if (base->header.exten_info.size() < 4) {
            LOG_ERROR << "header.exten_info size < 4";
            return E_DEFAULT;
        }

        return true;
    }

    int32_t node_request_service::on_node_create_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

		std::shared_ptr<matrix::service_core::node_create_task_req> req =
			std::dynamic_pointer_cast<matrix::service_core::node_create_task_req>(msg->get_content());
		if (req == nullptr) {
		    LOG_ERROR << "req is nullptr";
		    return E_DEFAULT;
		}

		if (!check_nonce(req->header.nonce)) {
			LOG_ERROR << "nonce check error ";
			return E_DEFAULT;
		}

		// check req body
		std::string req_additional;
        std::vector<std::string> req_peer_nodes;
        try {
            req_additional = req->body.additional;
            req_peer_nodes = req->body.peer_nodes_list;
        } catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
        }

		//验证签名
		std::string sign_msg = req->header.nonce + req_additional;
		if (!dbc::verify_sign(req->header.exten_info["sign"], sign_msg, req->header.exten_info["origin_id"])) {
			LOG_ERROR << "verify sign error." << req->header.exten_info["origin_id"];
			return E_DEFAULT;
		}

		// 检查是否命中当前节点
		bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
		if (hit_self) {
			return task_create(req);
		}
		else {
            req->header.path.push_back(CONF_MANAGER->get_node_id());
			CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
			return E_SUCCESS;
		}
	}

	int32_t node_request_service::on_node_start_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

	    std::shared_ptr<matrix::service_core::node_start_task_req> req =
			std::dynamic_pointer_cast<matrix::service_core::node_start_task_req>(msg->get_content());
		if (req == nullptr) {
            LOG_ERROR << "req is nullptr";
            return E_DEFAULT;
        }

		if (!check_nonce(req->header.nonce)) {
			LOG_ERROR << "nonce check failed";
			return E_DEFAULT;
		}

		// check req body
		std::string req_task_id;
		std::string req_additional;
        std::vector<std::string> req_peer_nodes;
		try {
		    req_task_id = req->body.task_id;
		    req_additional = req->body.additional;
		    req_peer_nodes = req->body.peer_nodes_list;
		} catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
		}

        if (!dbc::check_id(req_task_id)) {
            LOG_ERROR << "ai power provider service task_id error ";
            return E_DEFAULT;
        }

        std::string sign_msg = req_task_id + req->header.nonce + req_additional;
		if (!dbc::verify_sign(req->header.exten_info["sign"], sign_msg, req->header.exten_info["origin_id"])) {
			LOG_ERROR << "sign error." << req->header.exten_info["origin_id"];
			return E_DEFAULT;
		}

		// 检查是否命中当前节点
		bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
		if (hit_self) {
			return task_start(req);
		}
		else {
            req->header.path.push_back(CONF_MANAGER->get_node_id());
			CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
			return E_SUCCESS;
		}
	}

	int32_t node_request_service::on_node_stop_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

	    std::shared_ptr<matrix::service_core::node_stop_task_req> req = std::dynamic_pointer_cast<matrix::service_core::node_stop_task_req>(
			msg->get_content());
		if (req == nullptr) {
		    LOG_ERROR << "req is nullptr";
		    return E_DEFAULT;
		}

		if (!check_nonce(req->header.nonce)) {
			LOG_ERROR << "ai power provider service nonce error ";
			return E_DEFAULT;
		}

        // check req body
        std::string req_task_id;
        std::string req_additional;
        std::vector<std::string> req_peer_nodes;
        try {
            req_task_id = req->body.task_id;
            req_additional = req->body.additional;
            req_peer_nodes = req->body.peer_nodes_list;
        } catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
        }

		if (!dbc::check_id(req_task_id)) {
			LOG_ERROR << "ai power provider service on_stop_training_req task_id error ";
			return E_DEFAULT;
		}

		std::string sign_msg = req_task_id + req->header.nonce + req_additional;
		if (!dbc::verify_sign(req->header.exten_info["sign"], sign_msg, req->header.exten_info["origin_id"])) {
			LOG_ERROR << "sign error." << req->header.exten_info["origin_id"];
			return E_DEFAULT;
		}

		// 检查是否命中当前节点
		bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
		if (hit_self) {
			return task_stop(req);
		}
		else {
            req->header.path.push_back(CONF_MANAGER->get_node_id());
			CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
			return E_SUCCESS;
		}
	}

    int32_t node_request_service::on_node_restart_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

	    std::shared_ptr<matrix::service_core::node_restart_task_req> req =
                std::dynamic_pointer_cast<matrix::service_core::node_restart_task_req>(msg->get_content());
        if (req == nullptr) {
            LOG_ERROR << "req is nullptr";
            return E_DEFAULT;
        }

        if (!check_nonce(req->header.nonce)) {
            LOG_ERROR << "ai power provider service nonce error ";
            return E_DEFAULT;
        }

        // check req body
        std::string req_task_id;
        std::string req_additional;
        std::vector<std::string> req_peer_nodes;
        try {
            req_task_id = req->body.task_id;
            req_additional = req->body.additional;
            req_peer_nodes = req->body.peer_nodes_list;
        } catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
        }

        if (!dbc::check_id(req_task_id)) {
            LOG_ERROR << "ai power provider service task_id error ";
            return E_DEFAULT;
        }

        std::string sign_msg = req_task_id + req->header.nonce + req_additional;
        if (req->header.exten_info.size() < 3) {
            LOG_ERROR << "exten info error.";
            return E_DEFAULT;
        }
        if (!dbc::verify_sign(req->header.exten_info["sign"], sign_msg, req->header.exten_info["origin_id"])) {
            LOG_ERROR << "sign error." << req->header.exten_info["origin_id"];
            return E_DEFAULT;
        }

        // 检查是否命中当前节点
        bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
        if (hit_self) {
            return task_restart(req);
        }
        else {
            req->header.path.push_back(CONF_MANAGER->get_node_id());
            CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
            return E_SUCCESS;
        }
    }

    int32_t node_request_service::on_node_reset_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

	    std::shared_ptr<matrix::service_core::node_reset_task_req> req = std::dynamic_pointer_cast<matrix::service_core::node_reset_task_req>(
                msg->get_content());
        if (req == nullptr) return E_DEFAULT;

        if (!check_nonce(req->header.nonce)) {
            LOG_ERROR << "ai power provider service nonce error ";
            return E_DEFAULT;
        }

        // check req body
        std::string req_task_id;
        std::string req_additional;
        std::vector<std::string> req_peer_nodes;
        try {
            req_task_id = req->body.task_id;
            req_additional = req->body.additional;
            req_peer_nodes = req->body.peer_nodes_list;
        } catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
        }

        if (!dbc::check_id(req_task_id)) {
            LOG_ERROR << "ai power provider service on_stop_training_req task_id error ";
            return E_DEFAULT;
        }

        std::string sign_msg = req_task_id + req->header.nonce + req_additional;
        if (!dbc::verify_sign(req->header.exten_info["sign"], sign_msg, req->header.exten_info["origin_id"])) {
            LOG_ERROR << "sign error." << req->header.exten_info["origin_id"];
            return E_DEFAULT;
        }

        // 检查是否命中当前节点
        bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
        if (hit_self) {
            return task_reset(req);
        }
        else {
            req->header.path.push_back(CONF_MANAGER->get_node_id());
            CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
            return E_SUCCESS;
        }
    }

    int32_t node_request_service::on_node_delete_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

	    std::shared_ptr<matrix::service_core::node_delete_task_req> req = std::dynamic_pointer_cast<matrix::service_core::node_delete_task_req>(
                msg->get_content());
        if (req == nullptr) {
            LOG_ERROR << "req id nullptr";
            return E_DEFAULT;
        }

        if (!check_nonce(req->header.nonce)) {
            LOG_ERROR << "ai power provider service nonce error ";
            return E_DEFAULT;
        }

        // check req body
        std::string req_task_id;
        std::string req_additional;
        std::vector<std::string> req_peer_nodes;
        try {
            req_task_id = req->body.task_id;
            req_additional = req->body.additional;
            req_peer_nodes = req->body.peer_nodes_list;
        } catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
        }

        if (!dbc::check_id(req_task_id)) {
            LOG_ERROR << "ai power provider service on_stop_training_req task_id error ";
            return E_DEFAULT;
        }

        std::string sign_msg = req_task_id + req->header.nonce + req_additional;
        if (!dbc::verify_sign(req->header.exten_info["sign"], sign_msg, req->header.exten_info["origin_id"])) {
            LOG_ERROR << "sign error." << req->header.exten_info["origin_id"];
            return E_DEFAULT;
        }

        // 检查是否命中当前节点
        bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
        if (hit_self) {
            return task_delete(req);
        }
        else {
            req->header.path.push_back(CONF_MANAGER->get_node_id());
            CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
            return E_SUCCESS;
        }
    }

	int32_t node_request_service::on_node_task_logs_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

	    std::shared_ptr<node_task_logs_req> req_content = std::dynamic_pointer_cast<node_task_logs_req>(msg->get_content());
		if (req_content == nullptr) {
		    LOG_ERROR << "req is nullptr";
		    return E_DEFAULT;
		}

		if (!check_nonce(req_content->header.nonce)) {
			LOG_ERROR << "ai power provider service nonce error ";
			return E_DEFAULT;
		}

        // check req body
        std::string req_task_id;
        std::string req_additional;
        std::vector<std::string> req_peer_nodes;
        int8_t req_head_or_tail;
        int16_t req_number_of_lines;
        try {
            req_task_id = req_content->body.task_id;
            req_additional = req_content->body.additional;
            req_peer_nodes = req_content->body.peer_nodes_list;
            req_head_or_tail = req_content->body.head_or_tail;
            req_number_of_lines = req_content->body.number_of_lines;
        } catch (...) {
            LOG_ERROR << "req body error";
            return E_DEFAULT;
        }

		if (!dbc::check_id(req_task_id)) {
			LOG_ERROR << "taskid error ";
			return E_DEFAULT;
		}

		if (GET_LOG_HEAD != req_head_or_tail && GET_LOG_TAIL != req_head_or_tail) {
			LOG_ERROR << "ai power provider service on logs req log direction error";
			return E_DEFAULT;
		}

		if (req_number_of_lines > MAX_NUMBER_OF_LINES || req_number_of_lines < 0) {
			LOG_ERROR << "ai power provider service on logs req number of lines error: "
				<< req_number_of_lines;
			return E_DEFAULT;
		}

		std::string sign_req_msg =
			req_task_id + req_content->header.nonce + req_content->header.session_id + req_additional;
		if (!dbc::verify_sign(req_content->header.exten_info["sign"], sign_req_msg, req_content->header.exten_info["origin_id"])) {
			LOG_ERROR << "fake message. " << req_content->header.exten_info["origin_id"];
			return E_DEFAULT;
		}

		// 检查是否命中当前节点
		bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
		if (hit_self) {
			return task_logs(req_content);
		}
		else {
			req_content->header.path.push_back(CONF_MANAGER->get_node_id());
			CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
			return E_SUCCESS;
		}
	}

	int32_t node_request_service::on_node_list_task_req(std::shared_ptr<dbc::network::message>& msg) {
        if (!check_req_header(msg)) {
            LOG_ERROR << "req header check failed";
            return E_DEFAULT;
        }

		std::shared_ptr<node_list_task_req> req_content = std::dynamic_pointer_cast<node_list_task_req>(
			msg->get_content());
		if (req_content == nullptr) return E_DEFAULT;

		if (!check_nonce(req_content->header.nonce)) {
			LOG_ERROR << "ai power provider service nonce error ";
			return E_DEFAULT;
		}

		//check req.body
		std::string req_task_id;
        std::vector<std::string> req_peer_nodes;
        std::string req_additional;
		try {
		    req_task_id = req_content->body.task_id;
            req_peer_nodes = req_content->body.peer_nodes_list;
            req_additional = req_content->body.additional;
		} catch (...) {
		    LOG_ERROR << "req body error";
		    return E_DEFAULT;
		}

		if (!req_task_id.empty() && !dbc::check_id(req_task_id)) {
			LOG_ERROR << "task_id check failed";
			return E_DEFAULT;
		}

		std::string sign_msg = req_task_id + req_content->header.nonce + req_content->header.session_id + req_additional;
		if (!dbc::verify_sign(req_content->header.exten_info["sign"], sign_msg, req_content->header.exten_info["origin_id"])) {
			LOG_ERROR << "fake message. " << req_content->header.exten_info["origin_id"];
			return E_DEFAULT;
		}

		// 检查是否命中当前节点
		bool hit_self = hit_node(req_peer_nodes, CONF_MANAGER->get_node_id());
		if (hit_self) {
		    return task_list(req_content);
		} else {
			req_content->header.path.push_back(CONF_MANAGER->get_node_id());
			CONNECTION_MANAGER->broadcast_message(msg, msg->header.src_sid);
			return E_SUCCESS;
		}
	}

    int32_t node_request_service::on_get_task_queue_size_req(std::shared_ptr<dbc::network::message>& msg) {
        auto resp = std::make_shared<get_task_queue_size_resp_msg>();

        auto task_num = m_task_scheduler.GetRunningTaskSize();
        resp->set_task_size(task_num);

        auto resp_msg = std::dynamic_pointer_cast<dbc::network::message>(resp);

        TOPIC_MANAGER->publish<int32_t>(typeid(get_task_queue_size_resp_msg).name(), resp_msg);

        return E_SUCCESS;
    }

    static std::string generate_pwd() {
        char chr[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                       'H', 'I', 'J', 'K', 'L', 'M', 'N',
                       'O', 'P', 'Q', 'R', 'S', 'T',
                       'U', 'V', 'W', 'X', 'Y', 'Z',
                       'a', 'b', 'c', 'd', 'e', 'f', 'g',
                       'h', 'i', 'j', 'k', 'l', 'm', 'n',
                       'o', 'p', 'q', 'r', 's', 't',
                       'u', 'v', 'w', 'x', 'y', 'z',
                       '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };
        srand(time(NULL));
        string strpwd;
        int nlen = 10; //10位密码
        char buf[3] = { 0 };
        int idx0 = rand() % 52;
        sprintf(buf, "%c", chr[idx0]);
        strpwd.append(buf);

        int idx_0 = rand() % nlen;
        int idx_1 = rand() % nlen;
        int idx_2 = rand() % nlen;

        for (int i = 1; i < nlen; i++) {
            int idx;
            if (i == idx_0 || i == idx_1 || i == idx_2) {
                idx = rand() % 62;
            }
            else {
                idx = rand() % 62;
            }
            sprintf(buf, "%c", chr[idx]);
            strpwd.append(buf);
        }

        return strpwd;
    }

    int32_t node_request_service::task_create(const std::shared_ptr<matrix::service_core::node_create_task_req>& req) {
		if (req == nullptr) return E_DEFAULT;

		// 创建虚拟机
		std::string task_id = dbc::create_task_id();
		std::string login_password = generate_pwd();
		auto fresult = m_task_scheduler.CreateTask(task_id, login_password, req->body.additional);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        std::shared_ptr<matrix::service_core::node_create_task_rsp> rsp_content = std::make_shared<matrix::service_core::node_create_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_CREATE_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id + task_id + login_password;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        auto taskinfo = m_task_scheduler.FindTask(task_id);

        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);
        rsp_content->body.__set_task_id(task_id);
        rsp_content->body.__set_user_name("ubuntu");
        rsp_content->body.__set_login_password(login_password);
        std::string public_ip = run_shell("dig +short myip.opendns.com @resolver1.opendns.com");
        public_ip = string_util::rtrim(public_ip, '\n');
        rsp_content->body.__set_ip(public_ip);
        rsp_content->body.__set_ssh_port(taskinfo->ssh_port);
        struct tm _tm;
        time_t tt = taskinfo->create_time;
        localtime_r(&tt, &_tm);
        char buf[256] = {0};
        memset(buf, 0, sizeof(char) * 256);
        strftime(buf, sizeof(char) * 256, "%Y-%m-%d %H:%M:%S", &_tm);
        rsp_content->body.__set_create_time(buf);
        //todo: 填充系统信息
        rsp_content->body.__set_system_storage("350G");
        rsp_content->body.__set_data_storage("2T");
        rsp_content->body.__set_cpu_cores(std::to_string(taskinfo->hardware_resource.cpu_cores));
        rsp_content->body.__set_gpu_count(std::to_string(taskinfo->hardware_resource.gpu_count));
        rsp_content->body.__set_mem_size(std::to_string(taskinfo->hardware_resource.mem_rate));

        //rsp msg
        std::shared_ptr<dbc::network::message> resp_msg = std::make_shared<dbc::network::message>();
        resp_msg->set_name(NODE_CREATE_TASK_RSP);
        resp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(resp_msg);
        return E_SUCCESS;
    }

	int32_t node_request_service::task_start(const std::shared_ptr<matrix::service_core::node_start_task_req>& req) {
	    std::string task_id = req->body.task_id;
	    if (!dbc::check_id(task_id)) return E_DEFAULT;

	    auto fresult = m_task_scheduler.StartTask(task_id);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        std::shared_ptr<matrix::service_core::node_start_task_rsp> rsp_content = std::make_shared<matrix::service_core::node_start_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_START_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);

        //rsp msg
        std::shared_ptr<dbc::network::message> rsp_msg = std::make_shared<dbc::network::message>();
        rsp_msg->set_name(NODE_START_TASK_RSP);
        rsp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(rsp_msg);
        return E_SUCCESS;
	}

	int32_t node_request_service::task_stop(const std::shared_ptr<matrix::service_core::node_stop_task_req>& req) {
        std::string task_id = req->body.task_id;
        if (!dbc::check_id(task_id)) return E_DEFAULT;

        auto fresult = m_task_scheduler.StopTask(task_id);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        std::shared_ptr<matrix::service_core::node_stop_task_rsp> rsp_content = std::make_shared<matrix::service_core::node_stop_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_STOP_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);

        //rsp msg
        std::shared_ptr<dbc::network::message> rsp_msg = std::make_shared<dbc::network::message>();
        rsp_msg->set_name(NODE_STOP_TASK_RSP);
        rsp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(rsp_msg);
        return E_SUCCESS;
	}

    int32_t node_request_service::task_restart(const std::shared_ptr<matrix::service_core::node_restart_task_req>& req) {
        std::string task_id = req->body.task_id;
        if (!dbc::check_id(task_id)) return E_DEFAULT;

        auto fresult = m_task_scheduler.RestartTask(task_id);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        std::shared_ptr<matrix::service_core::node_restart_task_rsp> rsp_content = std::make_shared<matrix::service_core::node_restart_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_RESTART_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);

        //rsp msg
        std::shared_ptr<dbc::network::message> rsp_msg = std::make_shared<dbc::network::message>();
        rsp_msg->set_name(NODE_RESTART_TASK_RSP);
        rsp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(rsp_msg);
        return E_SUCCESS;
    }

    int32_t node_request_service::task_reset(const std::shared_ptr<matrix::service_core::node_reset_task_req>& req) {
        std::string task_id = req->body.task_id;
        if (!dbc::check_id(task_id)) return E_DEFAULT;

        auto fresult = m_task_scheduler.ResetTask(task_id);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        std::shared_ptr<matrix::service_core::node_reset_task_rsp> rsp_content = std::make_shared<matrix::service_core::node_reset_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_RESET_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);

        //rsp msg
        std::shared_ptr<dbc::network::message> rsp_msg = std::make_shared<dbc::network::message>();
        rsp_msg->set_name(NODE_RESET_TASK_RSP);
        rsp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(rsp_msg);
        return E_SUCCESS;
    }

    int32_t node_request_service::task_delete(const std::shared_ptr<matrix::service_core::node_delete_task_req>& req) {
        std::string task_id = req->body.task_id;
        if (!dbc::check_id(task_id)) return E_DEFAULT;

        auto fresult = m_task_scheduler.DeleteTask(task_id);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        std::shared_ptr<matrix::service_core::node_delete_task_rsp> rsp_content = std::make_shared<matrix::service_core::node_delete_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_DELETE_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);

        //rsp msg
        std::shared_ptr<dbc::network::message> rsp_msg = std::make_shared<dbc::network::message>();
        rsp_msg->set_name(NODE_DELETE_TASK_RSP);
        rsp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(rsp_msg);
        return E_SUCCESS;
    }

	int32_t node_request_service::task_logs(const std::shared_ptr<matrix::service_core::node_task_logs_req>& req) {
        std::string task_id = req->body.task_id;
        if (!dbc::check_id(task_id)) return E_DEFAULT;

        std::string log_content;
        auto fresult = m_task_scheduler.GetTaskLog(req->body.task_id, (ETaskLogDirection) req->body.head_or_tail,
                                                   req->body.number_of_lines, log_content);
        int32_t ret = std::get<0>(fresult);
        std::string ret_msg = std::get<1>(fresult);

        if (GET_LOG_HEAD == req->body.head_or_tail) {
            log_content = log_content.substr(0, MAX_LOG_CONTENT_SIZE);
        }
        else {
            size_t log_lenth = log_content.length();
            if (log_lenth > MAX_LOG_CONTENT_SIZE) {
                log_content = log_content.substr(log_lenth - MAX_LOG_CONTENT_SIZE, MAX_LOG_CONTENT_SIZE);
            }
        }

		std::shared_ptr<matrix::service_core::node_task_logs_rsp> rsp_content = std::make_shared<matrix::service_core::node_task_logs_rsp>();
		// header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
		rsp_content->header.__set_msg_name(NODE_TASK_LOGS_RSP);
		rsp_content->header.__set_nonce(dbc::create_nonce());
		rsp_content->header.__set_session_id(req->header.session_id);
		rsp_content->header.__set_path(req->header.path);
		std::map<std::string, std::string> exten_info;
		std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id + log_content;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
		rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(ret);
        rsp_content->body.__set_result_msg(ret_msg);
        rsp_content->body.__set_log_content(log_content);

        //rsp msg
		std::shared_ptr<dbc::network::message> rsp_msg = std::make_shared<dbc::network::message>();
        rsp_msg->set_name(NODE_TASK_LOGS_RSP);
        rsp_msg->set_content(rsp_content);
		CONNECTION_MANAGER->send_resp_message(rsp_msg);
		return E_SUCCESS;
	}

	int32_t node_request_service::task_list(const std::shared_ptr<matrix::service_core::node_list_task_req> &req) {
        int32_t result = E_SUCCESS;
        std::string result_msg = "task list successful";

	    std::string task_id = req->body.task_id;
        if (!task_id.empty() && !dbc::check_id(task_id)) {
            result = E_DEFAULT;
            result_msg = "task_id check failed";
        }

        std::vector<matrix::service_core::task_info> info_list;
        if (result == E_SUCCESS) {
            if (req->body.task_id.empty()) {
                std::vector<std::shared_ptr<TaskInfo>> task_list;
                m_task_scheduler.ListAllTask(task_list);
                for (auto &task : task_list) {
                    matrix::service_core::task_info tinfo;
                    tinfo.__set_task_id(task->task_id);
                    tinfo.__set_status(m_task_scheduler.GetTaskStatus(task->task_id));
                    tinfo.__set_login_password(task->login_password);
                    info_list.push_back(tinfo);
                }
            } else {
                auto task = m_task_scheduler.FindTask(req->body.task_id);
                if (nullptr != task) {
                    matrix::service_core::task_info tinfo;
                    tinfo.__set_task_id(task->task_id);
                    tinfo.__set_status(m_task_scheduler.GetTaskStatus(task->task_id));
                    tinfo.__set_login_password(task->login_password);
                    info_list.push_back(tinfo);
                } else {
                    result = E_DEFAULT;
                    result_msg = "task_id not exist";
                }
            }
        }

        std::shared_ptr<matrix::service_core::node_list_task_rsp> rsp_content =
                std::make_shared<matrix::service_core::node_list_task_rsp>();
        // header
        rsp_content->header.__set_magic(CONF_MANAGER->get_net_flag());
        rsp_content->header.__set_msg_name(NODE_LIST_TASK_RSP);
        rsp_content->header.__set_nonce(dbc::create_nonce());
        rsp_content->header.__set_session_id(req->header.session_id);
        rsp_content->header.__set_path(req->header.path);
        std::map<std::string, std::string> exten_info;
        std::string sign_msg = rsp_content->header.nonce + rsp_content->header.session_id;
        std::string sign = dbc::sign(sign_msg, CONF_MANAGER->get_node_private_key());
        exten_info["sign"] = sign;
        exten_info["sign_algo"] = ECDSA;
        time_t cur = std::time(nullptr);
        exten_info["sign_at"] = boost::str(boost::format("%d") % cur);
        exten_info["origin_id"] = CONF_MANAGER->get_node_id();
        rsp_content->header.__set_exten_info(exten_info);
        // body
        rsp_content->body.__set_result(result);
        rsp_content->body.__set_result_msg(result_msg);
        rsp_content->body.task_info_list.swap(info_list);

        //rsp msg
        std::shared_ptr<dbc::network::message> resp_msg = std::make_shared<dbc::network::message>();
        resp_msg->set_name(NODE_LIST_TASK_RSP);
        resp_msg->set_content(rsp_content);
        CONNECTION_MANAGER->send_resp_message(resp_msg);
        return E_SUCCESS;
	}

	int32_t node_request_service::on_training_task_timer(const std::shared_ptr<core_timer>& timer) {
	    m_task_scheduler.ProcessTask();

		static int count = 0;
		count++;
		if ((count % 10) == 0) {
			//m_task_schedule->update_gpu_info_from_proc();
		}

		return E_SUCCESS;
	}

    int32_t node_request_service::on_prune_task_timer(const std::shared_ptr<core_timer>& timer) {
	    m_task_scheduler.PruneTask();
	    return E_SUCCESS;
	}

    int32_t node_request_service::check_sign(const std::string& message, const std::string& sign,
                                             const std::string& origin_id, const std::string& sign_algo) {
        if (sign_algo != ECDSA) {
            LOG_ERROR << "sign_algorithm error.";
            return E_DEFAULT;
        }

        if (origin_id.empty() || sign.empty()) {
            LOG_ERROR << "sign error.";
            return E_DEFAULT;
        }

        return E_SUCCESS;
    }

    std::string node_request_service::get_task_id(const std::string& server_specification) {
        if (server_specification.empty()) {
            return "";
        }

        std::string task_id;
        if (!server_specification.empty()) {
            std::stringstream ss;
            ss << server_specification;
            boost::property_tree::ptree pt;

            try {
                boost::property_tree::read_json(ss, pt);
                if (pt.count("task_id") != 0) {
                    task_id = pt.get<std::string>("task_id");
                }
            }
            catch (...) {

            }
        }

        return task_id;
    }

    std::string node_request_service::format_logs(const std::string& raw_logs, uint16_t max_lines) {
        //docker logs has special format with each line of log:
        // 0x01 0x00  0x00 0x00 0x00 0x00 0x00 0x38
        //we should remove it
        //and ends with 0x30 0x0d 0x0a
        max_lines = (max_lines == 0) ? MAX_NUMBER_OF_LINES : max_lines;
        size_t size = raw_logs.size();
        vector<unsigned char> log_vector(size);

        int push_char_count = 0;
        const char* p = raw_logs.c_str();

        uint16_t line_count = 1;

        for (size_t i = 0; i < size;) {
            //0x30 0x0d 0x0a
            if ((i + 2 < size)
                && (0x30 == *p)
                && (0x0d == *(p + 1))
                && (0x0a == *(p + 2))) {
                break;
            }

            if (max_lines != 0 && line_count > max_lines) {
                break;
            }

            //skip: 0x01 0x00  0x00 0x00 0x00 0x00 0x00 0x38
            if ((i + 7 < size)
                && ((0x01 == *p) || (0x02 == *p))
                && (0x00 == *(p + 1))
                && (0x00 == *(p + 2))
                && (0x00 == *(p + 3))
                && (0x00 == *(p + 4))
                && (0x00 == *(p + 5))) {
                i += 8;
                p += 8;
                continue;
            }

            log_vector[push_char_count] = *p++;

            if (log_vector[push_char_count] == '\n') {
                line_count++;
            }

            ++push_char_count;
            i++;
        }

        std::string formatted_str;
        formatted_str.reserve(push_char_count);

        int i = 0;
        while (i < push_char_count) {
            formatted_str += log_vector[i++];
        }

        return formatted_str;
    }
}
