/*********************************************************************************
*  Copyright (c) 2017-2018 DeepBrainChain core team
*  Distributed under the MIT software license, see the accompanying
*  file COPYING or http://www.opensource.org/licenses/mit-license.php
* file name        :   ai_power_provider_service.h
* description    :   ai_power_provider_service
* date                  :   2018.04.05
* author            :   Bruce Feng
**********************************************************************************/
#pragma once

#include <leveldb/db.h>
#include <string>
#include "service_module.h"
#include "db/ai_db_types.h"
#include "container_client.h"
#include "task_common_def.h"
#include "document.h"
#include "oss_client.h"
#include <boost/process.hpp>
#include "image_manager.h"
#include "task_scheduling.h"
#include "container_worker.h"

#define AI_TRAINING_TASK_TIMER                                   "training_task"
//#define AI_TRAINING_TASK_TIMER_INTERVAL                        (30 * 1000)                                                 //30s timer
#define AI_PRUNE_TASK_TIMER                                      "prune_task"
#define AI_PRUNE_TASK_TIMER_INTERVAL                             (10 * 60 * 1000)                                                 //10min timer

using namespace matrix::core;
namespace bp = boost::process;

namespace ai {
    namespace dbc {
        class node_request_service : public service_module {
        public:
            node_request_service();

            ~node_request_service() override = default;

            std::string module_name() const override { return "node_request_service"; }

        protected:
            void init_timer() override;

            void init_invoker() override;

            void init_subscription() override;

            int32_t service_init(bpo::variables_map &options) override;

            int32_t service_exit() override;

            int32_t on_start_training_req(std::shared_ptr<message> &msg);

            int32_t task_start(std::shared_ptr<matrix::service_core::start_training_req> req);

            int32_t task_restart(std::shared_ptr<matrix::service_core::start_training_req> req);

            int32_t task_reboot(std::shared_ptr<matrix::service_core::start_training_req> req);

            int32_t on_stop_training_req(std::shared_ptr<message> &msg);

            int32_t task_stop(std::shared_ptr<matrix::service_core::stop_training_req> req);

            int32_t on_logs_req(const std::shared_ptr<message> &msg);

            int32_t task_logs(std::shared_ptr<matrix::service_core::logs_req> req);

            int32_t on_list_training_req(std::shared_ptr<message> &msg);

            int32_t on_get_task_queue_size_req(std::shared_ptr<message> &msg);

            int32_t on_training_task_timer(const std::shared_ptr<core_timer>& timer);

            int32_t on_prune_task_timer(std::shared_ptr<core_timer> timer);

            std::string format_logs(const std::string &raw_logs, uint16_t max_lines);

            int32_t check_sign(const std::string& message, const std::string &sign, const std::string &origin_id,
                               const std::string &sign_algo);

            std::string get_task_id(std::shared_ptr<matrix::service_core::start_training_req> req);

        protected:
            std::shared_ptr<task_scheduling> m_user_task_ptr = nullptr;

            std::shared_ptr<container_worker> m_container_worker = nullptr;
            std::shared_ptr<vm_worker> m_vm_worker = nullptr;

            // 重启宿主机
            std::shared_ptr<ai_training_task> m_reboot_task = nullptr;

            uint32_t m_training_task_timer_id;
            uint32_t m_prune_task_timer_id;
        };
    }
}
