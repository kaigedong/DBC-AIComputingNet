/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ai_db_TYPES_H
#define ai_db_TYPES_H

#include <iosfwd>
#include "protocol.h"
#include "TToString.h"

namespace ai { namespace dbc {

class ai_training_task;

class cmd_task_info;

typedef struct _ai_training_task__isset {
  _ai_training_task__isset() : master(false), server_specification(false), server_count(false), hyper_parameters(false) {}
  bool master :1;
  bool server_specification :1;
  bool server_count :1;
  bool hyper_parameters :1;
} _ai_training_task__isset;

class ai_training_task : public virtual ::apache::thrift::TBase {
 public:

  ai_training_task(const ai_training_task&);
  ai_training_task& operator=(const ai_training_task&);
  ai_training_task() : task_id(), select_mode(0), master(), server_specification(), server_count(0), training_engine(), code_dir(), entry_file(), data_dir(), checkpoint_dir(), hyper_parameters(), ai_user_node_id(), start_time(0), end_time(0), error_times(0), container_id(), received_time_stamp(0), status(0) {
  }

  virtual ~ai_training_task() throw();
  std::string task_id;
  int8_t select_mode;
  std::string master;
  std::vector<std::string>  peer_nodes_list;
  std::string server_specification;
  int32_t server_count;
  std::string training_engine;
  std::string code_dir;
  std::string entry_file;
  std::string data_dir;
  std::string checkpoint_dir;
  std::string hyper_parameters;
  std::string ai_user_node_id;
  int64_t start_time;
  int64_t end_time;
  int8_t error_times;
  std::string container_id;
  int64_t received_time_stamp;
  int8_t status;

  _ai_training_task__isset __isset;

  void __set_task_id(const std::string& val);

  void __set_select_mode(const int8_t val);

  void __set_master(const std::string& val);

  void __set_peer_nodes_list(const std::vector<std::string> & val);

  void __set_server_specification(const std::string& val);

  void __set_server_count(const int32_t val);

  void __set_training_engine(const std::string& val);

  void __set_code_dir(const std::string& val);

  void __set_entry_file(const std::string& val);

  void __set_data_dir(const std::string& val);

  void __set_checkpoint_dir(const std::string& val);

  void __set_hyper_parameters(const std::string& val);

  void __set_ai_user_node_id(const std::string& val);

  void __set_start_time(const int64_t val);

  void __set_end_time(const int64_t val);

  void __set_error_times(const int8_t val);

  void __set_container_id(const std::string& val);

  void __set_received_time_stamp(const int64_t val);

  void __set_status(const int8_t val);

  bool operator == (const ai_training_task & rhs) const
  {
    if (!(task_id == rhs.task_id))
      return false;
    if (!(select_mode == rhs.select_mode))
      return false;
    if (__isset.master != rhs.__isset.master)
      return false;
    else if (__isset.master && !(master == rhs.master))
      return false;
    if (!(peer_nodes_list == rhs.peer_nodes_list))
      return false;
    if (__isset.server_specification != rhs.__isset.server_specification)
      return false;
    else if (__isset.server_specification && !(server_specification == rhs.server_specification))
      return false;
    if (__isset.server_count != rhs.__isset.server_count)
      return false;
    else if (__isset.server_count && !(server_count == rhs.server_count))
      return false;
    if (!(training_engine == rhs.training_engine))
      return false;
    if (!(code_dir == rhs.code_dir))
      return false;
    if (!(entry_file == rhs.entry_file))
      return false;
    if (!(data_dir == rhs.data_dir))
      return false;
    if (!(checkpoint_dir == rhs.checkpoint_dir))
      return false;
    if (__isset.hyper_parameters != rhs.__isset.hyper_parameters)
      return false;
    else if (__isset.hyper_parameters && !(hyper_parameters == rhs.hyper_parameters))
      return false;
    if (!(ai_user_node_id == rhs.ai_user_node_id))
      return false;
    if (!(start_time == rhs.start_time))
      return false;
    if (!(end_time == rhs.end_time))
      return false;
    if (!(error_times == rhs.error_times))
      return false;
    if (!(container_id == rhs.container_id))
      return false;
    if (!(received_time_stamp == rhs.received_time_stamp))
      return false;
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const ai_training_task &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ai_training_task & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(ai_training_task &a, ai_training_task &b);

std::ostream& operator<<(std::ostream& out, const ai_training_task& obj);


class cmd_task_info : public virtual ::apache::thrift::TBase {
 public:

  cmd_task_info(const cmd_task_info&);
  cmd_task_info& operator=(const cmd_task_info&);
  cmd_task_info() : task_id(), create_time(0), result(), status(0) {
  }

  virtual ~cmd_task_info() throw();
  std::string task_id;
  int64_t create_time;
  std::string result;
  int8_t status;

  void __set_task_id(const std::string& val);

  void __set_create_time(const int64_t val);

  void __set_result(const std::string& val);

  void __set_status(const int8_t val);

  bool operator == (const cmd_task_info & rhs) const
  {
    if (!(task_id == rhs.task_id))
      return false;
    if (!(create_time == rhs.create_time))
      return false;
    if (!(result == rhs.result))
      return false;
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const cmd_task_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const cmd_task_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(cmd_task_info &a, cmd_task_info &b);

std::ostream& operator<<(std::ostream& out, const cmd_task_info& obj);

}} // namespace

#endif
