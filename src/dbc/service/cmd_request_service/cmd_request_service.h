#ifndef DBC_CMD_REQUEST_SERVICE_H
#define DBC_CMD_REQUEST_SERVICE_H

#include <string>
#include "service_module.h"
#include "cmd_message.h"

using namespace matrix::core;
namespace fs = boost::filesystem;

namespace dbc {
    class cmd_request_service : public service_module {
    public:
        cmd_request_service() = default;

        ~cmd_request_service() override = default;

        std::string module_name() const override {
            return "cmd_request_service";
        }

    protected:
        void init_timer() override;

        void init_invoker() override;

        void init_subscription() override;

        int32_t service_init(bpo::variables_map &options) override;

        // create task
        int32_t on_cmd_create_task_req(std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_create_task_req_msg(const std::shared_ptr<::cmd_create_task_req> &cmd_req);

        int32_t on_node_create_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_create_task_timer(const std::shared_ptr<core_timer> &timer);

        // start task
        int32_t on_cmd_start_task_req(std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_start_task_req_msg(const std::shared_ptr<::cmd_start_task_req> &cmd_req);

        int32_t on_node_start_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_start_task_timer(const std::shared_ptr<core_timer> &timer);

        // stop task
        int32_t on_cmd_stop_task_req(const std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_stop_task_req_msg(const std::shared_ptr<::cmd_stop_task_req> &cmd_req);

        int32_t on_node_stop_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_stop_task_timer(const std::shared_ptr<core_timer> &timer);

        // restart task
        int32_t on_cmd_restart_task_req(std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_restart_task_req_msg(const std::shared_ptr<::cmd_restart_task_req> &cmd_req);

        int32_t on_node_restart_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_restart_task_timer(const std::shared_ptr<core_timer> &timer);

        // reset task
        int32_t on_cmd_reset_task_req(const std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_reset_task_req_msg(const std::shared_ptr<::cmd_reset_task_req> &cmd_req);

        int32_t on_node_reset_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_reset_task_timer(const std::shared_ptr<core_timer> &timer);

        // destroy task
        int32_t on_cmd_destroy_task_req(const std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_destroy_task_req_msg(const std::shared_ptr<::cmd_destroy_task_req> &cmd_req);

        int32_t on_node_destroy_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_destroy_task_timer(const std::shared_ptr<core_timer> &timer);

        // task logs
        int32_t on_cmd_task_logs_req(const std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_task_logs_req_msg(const std::shared_ptr<::cmd_task_logs_req> &cmd_req);

        int32_t on_node_task_logs_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_task_logs_timer(std::shared_ptr<core_timer> timer);

        // list task
        int32_t on_cmd_list_task_req(const std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_list_task_req_msg(const std::shared_ptr<::cmd_list_task_req> &cmd_req);

        int32_t on_node_list_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_list_task_timer(const std::shared_ptr<core_timer> &timer);

        // modify task
        int32_t on_cmd_modify_task_req(const std::shared_ptr<dbc::network::message> &msg);

        std::shared_ptr<dbc::network::message> create_node_modify_task_req_msg(const std::shared_ptr<::cmd_modify_task_req> &cmd_req);

        int32_t on_node_modify_task_rsp(std::shared_ptr<dbc::network::message> &msg);

        int32_t on_node_modify_task_timer(const std::shared_ptr<core_timer> &timer);


        int32_t on_binary_forward(std::shared_ptr<dbc::network::message> &msg);

        int32_t validate_cmd_training_task_conf(const bpo::variables_map &vm, std::string &error);

        int32_t validate_ipfs_path(const std::string &path_arg);

        int32_t validate_entry_file_name(const std::string &entry_file_name);

        bool precheck_msg(std::shared_ptr<dbc::network::message> &msg);
    };
}

#endif
