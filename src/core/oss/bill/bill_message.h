/*********************************************************************************
*  Copyright (c) 2017-2018 DeepBrainChain core team
*  Distributed under the MIT software license, see the accompanying
*  file COPYING or http://www.opensource.org/licenses/mit-license.php
* file name        container_message.h
* description    container message definition
* date                  2018.07.07
* author            Regulus
**********************************************************************************/

#pragma once

#include "common.h"
#include "container_message.h"
#include "oss_common_def.h"

namespace matrix
{
    namespace core
    {
        const int64_t DEFAULT_AUTH_REPORT_CYTLE = 60;
        const int64_t DEFAULT_AUTH_REPORT_INTERVAL = DEFAULT_AUTH_REPORT_CYTLE * 60 * 1000;
        const int64_t MAX_AUTH_REPORT_CYTLE = 3* DEFAULT_AUTH_REPORT_CYTLE;
        class auth_task_req
        {
        public:

            std::string mining_node_id;

            std::string ai_user_node_id;

            std::string task_id;

            std::string start_time;
           
            std::string task_state;
            
            std::string end_time;

            std::string sign_type;

            std::string sign;

            std::string to_string();
        };

        class auth_task_resp
        {
        public:

            int64_t status = OSS_SUCCESS;

            std::string contract_state;

            int64_t report_cycle = DEFAULT_AUTH_REPORT_CYTLE;
            void from_string(const std::string & buf);
        };
    }

}