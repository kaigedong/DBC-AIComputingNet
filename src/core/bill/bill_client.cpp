/*********************************************************************************
*  Copyright (c) 2017-2018 DeepBrainChain core team
*  Distributed under the MIT software license, see the accompanying
*  file COPYING or http://www.opensource.org/licenses/mit-license.php
* file name        :   container_client.cpp
* description    :   container client for definition
* date                  :   2018.07.20
* author            :   Regulus
**********************************************************************************/

#include "bill_client.h"
#include "common.h"

#include <event2/event.h>
#include <event2/http.h>

#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include "error/en.h"

namespace matrix
{
    namespace core
    {

        bill_client::bill_client(const std::string & url, const std::string & crt)
            : m_url(url),
            m_crt(crt),
            m_http_client(url, crt)
        {
        }
            
        //auth_task
        std::shared_ptr<auth_task_resp> bill_client::post_auth_task(std::shared_ptr<auth_task_req> req)
        {
            //req content, headers, resp
            std::string && req_content = req->to_string();

            LOG_DEBUG << "auth req info:" << req_content;

            kvs headers;
            headers.push_back({ "Content-Type", "application/json;charset=UTF-8" });
            headers.push_back({ "Host", m_http_client.get_remote_host()});
            http_response resp;
            int32_t ret = E_SUCCESS;
            std::string end_point = m_http_client.get_uri();
            try
            {
                ret = m_http_client.post(end_point, headers, req_content, resp);
            }
            catch (const std::exception & e)
            {
                LOG_ERROR << "auth task error: " << e.what();
                return nullptr;
            }

            if (E_SUCCESS != ret)
            {
                LOG_ERROR << "auth failed: " << resp.body;
                return nullptr;
            }

            LOG_DEBUG << "auth resp info: " << resp.body;

            //parse resp
            std::shared_ptr<auth_task_resp> auth_resp = std::make_shared<auth_task_resp>();
            auth_resp->from_string(resp.body);

            return auth_resp;
        }

    }

}