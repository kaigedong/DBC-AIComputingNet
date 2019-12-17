/*********************************************************************************
*  Copyright (c) 2017-2018 DeepBrainChain core team
*  Distributed under the MIT software license, see the accompanying
*  file COPYING or http://www.opensource.org/licenses/mit-license.php
* file name        :   oss_task_manager.cpp
* description    :   oss_task_manager for implementation
* date                  :   2018.10.17
* author            :   Regulus
**********************************************************************************/
#include "common.h"
#include "task_scheduling.h"
#include "util.h"
#include "utilstrencodings.h"
#include "task_common_def.h"
#include "server.h"
#include <boost/format.hpp>
#include <regex>
#include "time_util.h"
#include "url_validator.h"

namespace ai
{
    namespace dbc
    {
        task_scheduling::task_scheduling(std::shared_ptr<container_worker> container_worker_ptr)
        {
            m_container_worker = container_worker_ptr;
        }


        int32_t task_scheduling::init_db(std::string db_name)
        {
            auto rtn = m_task_db.init_db(env_manager::get_db_path(),db_name);
            if (E_SUCCESS != rtn)
            {
                return rtn;
            }

            try
            {
                load_task();
            }
            catch (const std::exception & e)
            {
                LOG_ERROR << "load task from db error: " << e.what();
                return E_DEFAULT;
            }

            return E_SUCCESS;

        }


        int32_t task_scheduling::update_task(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task)
            {
                return E_DEFAULT;
            }
            if (task->entry_file.empty() || task->code_dir.empty() || task->task_id.empty())
            {
                LOG_DEBUG << "task config error.";
                return E_DEFAULT;
            }

          //  auto state = get_task_state(task);

                if (task->container_id.empty())
                {

                    LOG_INFO << "container_id null. container_id";

                    return E_DEFAULT;
                }

                std::shared_ptr<update_container_config> config = m_container_worker->get_update_container_config(task);
                int32_t ret= CONTAINER_WORKER_IF->update_container(task->container_id, config);


                if (ret != E_SUCCESS)
                {
                    LOG_ERROR << "update task error. Task id:" << task->task_id;
                    return E_DEFAULT;
                }

                LOG_INFO << "update task success. Task id:" << task->task_id;
//                task->__set_update_time(time_util::get_time_stamp_ms());
                task->__set_start_time(time_util::get_time_stamp_ms());
                task->__set_status(task_running);
                task->error_times = 0;

   //             task->__set_memory(config->memory);
   //             task->__set_memory_swap(config->memory_swap);
               // task->__set_gpus(config->env);
                m_task_db.write_task_to_db(task);


            return E_SUCCESS;
        }

        int32_t task_scheduling::update_task_commit_image(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task)
            {
                return E_DEFAULT;
            }

            if (task->entry_file.empty() || task->code_dir.empty() || task->task_id.empty())
            {
                LOG_DEBUG << "task config error.";
                return E_DEFAULT;
            }

            std::string autodbcimage_version=m_container_worker->get_autodbcimage_version(task);
            LOG_INFO << "autodbcimage_version" << autodbcimage_version;
            if(autodbcimage_version=="")
            {
                return E_DEFAULT;
            }
            std::string image_id = CONTAINER_WORKER_IF->get_commit_image(task->container_id,autodbcimage_version);
            if(image_id!="")
            {
                std::string training_engine_original=task->training_engine;
                std::string training_engine_new="www.dbctalk.ai:5000/dbc-free-container:autodbcimage_"+task->container_id.substr(0,12)+autodbcimage_version;
                task->__set_training_engine(training_engine_new);
                LOG_INFO << "training_engine_original:" << training_engine_original;
                LOG_INFO << "training_engine_new:" << "www.dbctalk.ai:5000/dbc-free-container:autodbcimage_"+task->container_id.substr(0,12)+autodbcimage_version;
                int32_t status=start_task_from_new_image(task,autodbcimage_version,training_engine_new);

                if(status!= E_NO_START_CONTAINER &&status!= E_SUCCESS)
                {
                    task->__set_training_engine(training_engine_original);
                    return E_DEFAULT;
                }


            }


            return E_SUCCESS;
        }

       //from new image
        int32_t task_scheduling::start_task_from_new_image(std::shared_ptr<ai_training_task> task,std::string autodbcimage_version,std::string training_engine_new)
        {
            if (nullptr == task)
            {
                return E_DEFAULT;
            }



            std::shared_ptr<container_inspect_response> resp = CONTAINER_WORKER_IF->inspect_container(task->container_id);
            std:string old_container_id=task->container_id;
            if (true == resp->state.running)
            {
                if(E_SUCCESS==CONTAINER_WORKER_IF->stop_container(old_container_id))
                {
                    LOG_INFO << "stop container success , task id:" << old_container_id;



                   // return E_SUCCESS;
                } else
                {
                    LOG_INFO << "stop container failure , task id:" << task->task_id;
                    return E_DEFAULT;

                }
            }


            if (E_SUCCESS != create_task_from_image(task,autodbcimage_version))
            {
                LOG_ERROR << "create task error";
                CONTAINER_WORKER_IF->delete_image(training_engine_new);//delete new image
                CONTAINER_WORKER_IF->start_container(task->container_id);//start original container_id
                return E_DEFAULT;
            }else{

                if(E_SUCCESS!=CONTAINER_WORKER_IF->remove_container(old_container_id))//delete old container
                {
                    CONTAINER_WORKER_IF->remove_container(task->container_id);//delete new docker
                    CONTAINER_WORKER_IF->delete_image(training_engine_new);//delete new image
                    CONTAINER_WORKER_IF->start_container(old_container_id);//start original container_id
                    return E_DEFAULT;
                }
                LOG_INFO << "delete old container success , task id:" << old_container_id;
                if(E_SUCCESS!=CONTAINER_WORKER_IF->rename_container(task->task_id,autodbcimage_version))
                {
                    LOG_INFO << "rename container failure";
                  //  CONTAINER_WORKER_IF->remove_container(task->container_id);//delete new docker
                 //   LOG_INFO << "delete new  container ";
                  //  CONTAINER_WORKER_IF->delete_image(training_engine_new);//delete new image
                 //   CONTAINER_WORKER_IF->start_container(old_container_id);//start original container_id
                    CONTAINER_WORKER_IF->rename_container(task->task_id,autodbcimage_version);
                   // CONTAINER_WORKER_IF->start_container(task->container_id);//start new container_id
                   // return E_DEFAULT;
                }

                LOG_INFO << "rename container success , task id:" << task->task_id;
            }
            LOG_INFO << "start_task_from_new_image success. Task id:" ;

            int32_t ret = CONTAINER_WORKER_IF->start_container(task->container_id);//start new container_id

            if (ret != E_SUCCESS)
            {
                LOG_ERROR << "Start task error. Task id:" << task->task_id;
                return E_NO_START_CONTAINER;
            }

            LOG_INFO << "start_task_from_new_image success. Task id:" << task->task_id;
            task->__set_start_time(time_util::get_time_stamp_ms());
            task->__set_status(task_running);
            task->error_times = 0;
            LOG_INFO << "update task status:" << "task_running";
            m_task_db.write_task_to_db(task);
            LOG_INFO << "update task status:" << "write_task_to_db";
            LOG_INFO << "update E_SUCCESS:" << E_SUCCESS;
            return E_SUCCESS;
        }

        vector<string> split(const string& str, const string& delim) {
            vector<string> res;
            if("" == str) return res;
            //先将要切割的字符串从string类型转换为char*类型
            char * strs = new char[str.length() + 1] ; //不要忘了
            strcpy(strs, str.c_str());

            char * d = new char[delim.length() + 1];
            strcpy(d, delim.c_str());

            char *p = strtok(strs, d);
            while(p) {
                string s = p; //分割得到的字符串转换为string类型
                res.push_back(s); //存入结果数组
                p = strtok(NULL, d);
            }

            return res;
        }


        int32_t task_scheduling::create_task_from_image(std::shared_ptr<ai_training_task> task,std::string  autodbcimage_version)
        {
            if (nullptr == task)
            {
                return E_DEFAULT;
            }

            if (task->entry_file.empty() || task->code_dir.empty() || task->task_id.empty())
            {
                LOG_INFO << "task config error.";
                return E_DEFAULT;
            }

         //   std::string container_id_original=task->container_id;
            std::shared_ptr<container_config> config = m_container_worker->get_container_config_from_image(task);
            std::string task_id=task->task_id;

           // vector<string> vData=split(task_id, "_DBC_");
           // std::string sub_task_id=vData[0];
            std::shared_ptr<container_create_resp> resp = CONTAINER_WORKER_IF->create_container(config, task_id,autodbcimage_version);

            if (resp != nullptr && !resp->container_id.empty())
            {
                task->__set_container_id(resp->container_id);
                LOG_INFO << "create from_image task success. task id:" << task->task_id << " container id:" << task->container_id;

                return E_SUCCESS;
            }
            else
            {
                LOG_ERROR << "create task failed. task id:" << task->task_id;
            }
            return E_DEFAULT;
        }


        int32_t task_scheduling::create_task(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task)
            {
                return E_DEFAULT;
            }

            if (task->entry_file.empty() || task->code_dir.empty() || task->task_id.empty())
            {
                LOG_INFO << "task config error.";
                return E_DEFAULT;
            }


            std::shared_ptr<container_config> config = m_container_worker->get_container_config(task);
            std::shared_ptr<container_create_resp> resp = CONTAINER_WORKER_IF->create_container(config, task->task_id,"");
            if (resp != nullptr && !resp->container_id.empty())
            {
                task->__set_container_id(resp->container_id);
                LOG_INFO << "create task success. task id:" << task->task_id << " container id:" << task->container_id;

                return E_SUCCESS;
            }
            else
            {
                LOG_ERROR << "create task failed. task id:" << task->task_id;
            }
            return E_DEFAULT;
        }

        int32_t task_scheduling::start_task(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task)
            {
                return E_SUCCESS;
            }




            auto state = get_task_state(task);
            if (DBC_TASK_RUNNING == state)
            {
                if (task->status != task_running)
                {
                    task->__set_status(task_running);
                    m_task_db.write_task_to_db(task);
                }
                LOG_DEBUG << "task have been running, do not need to start. task id:" << task->task_id;
                return E_SUCCESS;
            }

            //if image do not exist, then pull it
            if (E_IMAGE_NOT_FOUND == CONTAINER_WORKER_IF->exist_docker_image(task->training_engine))
            {
                return start_pull_image(task);
            }

            bool is_container_existed = (!task->container_id.empty());
            if (!is_container_existed)
            {
                //if container_id does not exist, means dbc need to create container
                if (E_SUCCESS != create_task(task))
                {
                    LOG_ERROR << "create task error";
                    return E_DEFAULT;
                }
            }


            // update container's parameter if
            std::string path = env_manager::get_home_path().generic_string() + "/container/parameters";
            std::string text = "task_id=" + task->task_id + "\n";

            LOG_INFO << " container_id: " << task->container_id << " task_id: " << task->task_id;

            if (is_container_existed)
            {
                // server_specification indicates the container to be reused for this task
                // needs to indicate container run with different parameters
                text += ("code_dir=" + task->code_dir + "\n");

                if (task->server_specification == "restart")
                {
                    // use case: restart a task
                    text += ("restart=true\n");
                }
            }


            if (!file_util::write_file(path, text))
            {
                LOG_ERROR << "fail to refresh task's code_dir before reusing existing container for new task "
                          << task->task_id;
                return E_DEFAULT;
            }


            int32_t ret = CONTAINER_WORKER_IF->start_container(task->container_id);

            if (ret != E_SUCCESS)
            {
                LOG_ERROR << "Start task error. Task id:" << task->task_id;
                return E_DEFAULT;
            }

            LOG_INFO << "start task success. Task id:" << task->task_id;
            task->__set_start_time(time_util::get_time_stamp_ms());
            task->__set_status(task_running);
            task->error_times = 0;
            LOG_INFO << "task status:" << "task_running";
            m_task_db.write_task_to_db(task);
            LOG_INFO << "task status:" << "write_task_to_db";
            LOG_INFO << "E_SUCCESS:" << E_SUCCESS;
            return E_SUCCESS;
        }

        int32_t task_scheduling::stop_task(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task || task->container_id.empty())
            {
                return E_SUCCESS;
            }
            LOG_INFO << "stop task " << task->task_id;
            task->__set_end_time(time_util::get_time_stamp_ms());
            m_task_db.write_task_to_db(task);
            return CONTAINER_WORKER_IF->stop_container(task->container_id);
        }

        int32_t task_scheduling::delete_task(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task)
            {
                return E_SUCCESS;
            }

            try
            {
                if (DBC_TASK_RUNNING == get_task_state(task))
                {
                    stop_task(task);
                }

                if (!task->container_id.empty())
                {
                    CONTAINER_WORKER_IF->remove_container(task->container_id);
                }

                m_task_db.delete_task(task);
            }
            catch (...)
            {
                LOG_ERROR << "delete task abnormal";
                return E_DEFAULT;
            }

            return E_SUCCESS;
        }


        TASK_STATE task_scheduling::get_task_state(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task || task->task_id.empty())
            {
                return DBC_TASK_NULL;
            }

            // inspect container
            std::string container_id = task->task_id;

            // container can be started again by task delivered latter,
            // in that case, the container's id and name keeps the original value, then new task's id and container's name does not equal any more.
            if(!task->container_id.empty())
            {
                container_id = task->container_id;
            }

            std::shared_ptr<container_inspect_response> resp = CONTAINER_WORKER_IF->inspect_container(container_id);
            if (nullptr == resp)
            {
                task->__set_container_id("");
                return DBC_TASK_NOEXIST;
            }
            
            //local db may be deleted, but task is running, then container_id is empty.
            if (!resp->id.empty() && ( task->container_id.empty() || resp->id != task->container_id))
            {
                task->__set_container_id(resp->id);
            }

            if (true == resp->state.running)
            {
                return DBC_TASK_RUNNING;
            }

            return DBC_TASK_STOPPED;
        }



        int32_t task_scheduling::start_pull_image(std::shared_ptr<ai_training_task> task)
        {
            if (nullptr == task || task->training_engine.empty())
            {
                return E_SUCCESS;
            }

            //check local evn.
            auto ret = m_container_worker->can_pull_image();
            if (E_SUCCESS != m_container_worker->can_pull_image())
            {
                return ret;
            }

            if (nullptr == m_pull_image_mng)
            {
                m_pull_image_mng = std::make_shared<image_manager>();
            }

            //if the task pulling image is not the task need image.
            if (!m_pull_image_mng->get_pull_image_name().empty()
                && m_pull_image_mng->get_pull_image_name() != task->training_engine)
            {
                if (PULLING == m_pull_image_mng->check_pull_state())
                {
                    m_pull_image_mng->terminate_pull();
                }
            }

            //if training_engine is pulling, then return.
            if (PULLING == m_pull_image_mng->check_pull_state())
            {
                return E_SUCCESS;
            }

            //start pulling
            if (E_SUCCESS != m_pull_image_mng->start_pull(task->training_engine))
            {
                LOG_ERROR << "task engine pull fail. engine:" << task->training_engine;
                return E_PULLING_IMAGE;
            }

            m_pull_image_mng->set_start_time(time_util::get_time_stamp_ms());

            if (task_queueing == task->status)
            {
                task->__set_status(task_pulling_image);
                LOG_DEBUG << "docker pulling image. change status to " << to_training_task_status_string(task->status)
                    << " task id:" << task->task_id << " image engine:" << task->training_engine;
                m_task_db.write_task_to_db(task);
            }

            return E_SUCCESS;
        }

        int32_t task_scheduling::stop_pull_image(std::shared_ptr<ai_training_task> task)
        {
            if (!m_pull_image_mng)
            {
                return E_SUCCESS;
            }

            if (task->training_engine != m_pull_image_mng->get_pull_image_name())
            {
                LOG_ERROR << "pull image is not " << task->training_engine;
                return E_SUCCESS;
            }

            LOG_INFO << "terminate pull " << task->training_engine;
            m_pull_image_mng->terminate_pull();

            return E_SUCCESS;
        }
    }
}