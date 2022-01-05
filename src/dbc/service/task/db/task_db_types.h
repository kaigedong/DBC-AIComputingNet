/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef TASK_DB_TYPES_H
#define TASK_DB_TYPES_H

#include <iosfwd>
#include "network/protocol/protocol.h"
#include "network/protocol/service_message.h"
#include "util/TToString.h"

namespace dbc {

    class HardwareResource;

    class TaskInfo;

    class rent_task;

    typedef struct _HardwareResource__isset {
        _HardwareResource__isset() : gpu_count(false), cpu_cores(false), mem_rate(false) {}
        bool gpu_count :1;
        bool cpu_cores :1;
        bool mem_rate :1;
    } _HardwareResource__isset;

    class HardwareResource : public virtual ::apache::thrift::TBase {
    public:

        HardwareResource(const HardwareResource&);
        HardwareResource& operator=(const HardwareResource&);
        HardwareResource() : gpu_count(0), cpu_cores(0), mem_rate(0) {
        }

        virtual ~HardwareResource() throw();
        int32_t gpu_count;
        int32_t cpu_cores;
        double mem_rate;

        _HardwareResource__isset __isset;

        void __set_gpu_count(const int32_t val);

        void __set_cpu_cores(const int32_t val);

        void __set_mem_rate(const double val);

        bool operator == (const HardwareResource & rhs) const
        {
            if (__isset.gpu_count != rhs.__isset.gpu_count)
                return false;
            else if (__isset.gpu_count && !(gpu_count == rhs.gpu_count))
                return false;
            if (__isset.cpu_cores != rhs.__isset.cpu_cores)
                return false;
            else if (__isset.cpu_cores && !(cpu_cores == rhs.cpu_cores))
                return false;
            if (__isset.mem_rate != rhs.__isset.mem_rate)
                return false;
            else if (__isset.mem_rate && !(mem_rate == rhs.mem_rate))
                return false;
            return true;
        }
        bool operator != (const HardwareResource &rhs) const {
            return !(*this == rhs);
        }

        bool operator < (const HardwareResource & ) const;

        uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
        uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

        virtual void printTo(std::ostream& out) const;
    };

    void swap(HardwareResource &a, HardwareResource &b);

    std::ostream& operator<<(std::ostream& out, const HardwareResource& obj);

    typedef struct _TaskInfo__isset {
        _TaskInfo__isset() : image_name(false), login_password(false), ssh_port(false), status(false), operation(false), create_time(false), last_start_time(false), last_stop_time(false), hardware_resource(false), vm_xml(false), vm_xml_url(false), data_file_name(false), operation_system(false), bios_mode(false), rdp_port(false), custom_port(false), multicast(false) {}
        bool image_name :1;
        bool login_password :1;
        bool ssh_port :1;
        bool status :1;
        bool operation :1;
        bool create_time :1;
        bool last_start_time :1;
        bool last_stop_time :1;
        bool hardware_resource :1;
        bool vm_xml :1;
        bool vm_xml_url :1;
        bool data_file_name :1;
        bool operation_system :1;
        bool bios_mode :1;
        bool rdp_port :1;
        bool custom_port :1;
        bool multicast :1;
    } _TaskInfo__isset;

    class TaskInfo : public virtual ::apache::thrift::TBase {
    public:

        TaskInfo(const TaskInfo&);
        TaskInfo& operator=(const TaskInfo&);
        TaskInfo() : task_id(), image_name(), login_password(), ssh_port(), status(0), operation(0), create_time(0), last_start_time(0), last_stop_time(0), vm_xml(), vm_xml_url(), data_file_name(), operation_system(), bios_mode(), rdp_port() {
        }

        virtual ~TaskInfo() throw();
        std::string task_id;
        std::string image_name;
        std::string login_password;
        std::string ssh_port;
        int32_t status;
        int32_t operation;
        int64_t create_time;
        int64_t last_start_time;
        int64_t last_stop_time;
        HardwareResource hardware_resource;
        std::string vm_xml;
        std::string vm_xml_url;
        std::string data_file_name;
        std::string operation_system;
        std::string bios_mode;
        std::string rdp_port;
        std::vector<std::string>  custom_port;
        std::vector<std::string>  multicast;

        _TaskInfo__isset __isset;

        void __set_task_id(const std::string& val);

        void __set_image_name(const std::string& val);

        void __set_login_password(const std::string& val);

        void __set_ssh_port(const std::string& val);

        void __set_status(const int32_t val);

        void __set_operation(const int32_t val);

        void __set_create_time(const int64_t val);

        void __set_last_start_time(const int64_t val);

        void __set_last_stop_time(const int64_t val);

        void __set_hardware_resource(const HardwareResource& val);

        void __set_vm_xml(const std::string& val);

        void __set_vm_xml_url(const std::string& val);

        void __set_data_file_name(const std::string& val);

        void __set_operation_system(const std::string& val);

        void __set_bios_mode(const std::string& val);

        void __set_rdp_port(const std::string& val);

        void __set_custom_port(const std::vector<std::string> & val);

        void __set_multicast(const std::vector<std::string> & val);

        bool operator == (const TaskInfo & rhs) const
        {
            if (!(task_id == rhs.task_id))
                return false;
            if (__isset.image_name != rhs.__isset.image_name)
                return false;
            else if (__isset.image_name && !(image_name == rhs.image_name))
                return false;
            if (__isset.login_password != rhs.__isset.login_password)
                return false;
            else if (__isset.login_password && !(login_password == rhs.login_password))
                return false;
            if (__isset.ssh_port != rhs.__isset.ssh_port)
                return false;
            else if (__isset.ssh_port && !(ssh_port == rhs.ssh_port))
                return false;
            if (__isset.status != rhs.__isset.status)
                return false;
            else if (__isset.status && !(status == rhs.status))
                return false;
            if (__isset.operation != rhs.__isset.operation)
                return false;
            else if (__isset.operation && !(operation == rhs.operation))
                return false;
            if (__isset.create_time != rhs.__isset.create_time)
                return false;
            else if (__isset.create_time && !(create_time == rhs.create_time))
                return false;
            if (__isset.last_start_time != rhs.__isset.last_start_time)
                return false;
            else if (__isset.last_start_time && !(last_start_time == rhs.last_start_time))
                return false;
            if (__isset.last_stop_time != rhs.__isset.last_stop_time)
                return false;
            else if (__isset.last_stop_time && !(last_stop_time == rhs.last_stop_time))
                return false;
            if (!(hardware_resource == rhs.hardware_resource))
                return false;
            if (__isset.vm_xml != rhs.__isset.vm_xml)
                return false;
            else if (__isset.vm_xml && !(vm_xml == rhs.vm_xml))
                return false;
            if (__isset.vm_xml_url != rhs.__isset.vm_xml_url)
                return false;
            else if (__isset.vm_xml_url && !(vm_xml_url == rhs.vm_xml_url))
                return false;
            if (__isset.data_file_name != rhs.__isset.data_file_name)
                return false;
            else if (__isset.data_file_name && !(data_file_name == rhs.data_file_name))
                return false;
            if (__isset.operation_system != rhs.__isset.operation_system)
                return false;
            else if (__isset.operation_system && !(operation_system == rhs.operation_system))
                return false;
            if (__isset.bios_mode != rhs.__isset.bios_mode)
                return false;
            else if (__isset.bios_mode && !(bios_mode == rhs.bios_mode))
                return false;
            if (__isset.rdp_port != rhs.__isset.rdp_port)
                return false;
            else if (__isset.rdp_port && !(rdp_port == rhs.rdp_port))
                return false;
            if (__isset.custom_port != rhs.__isset.custom_port)
                return false;
            else if (__isset.custom_port && !(custom_port == rhs.custom_port))
                return false;
            if (__isset.multicast != rhs.__isset.multicast)
                return false;
            else if (__isset.multicast && !(multicast == rhs.multicast))
                return false;
            return true;
        }
        bool operator != (const TaskInfo &rhs) const {
            return !(*this == rhs);
        }

        bool operator < (const TaskInfo & ) const;

        uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
        uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

        virtual void printTo(std::ostream& out) const;
    };

    void swap(TaskInfo &a, TaskInfo &b);

    std::ostream& operator<<(std::ostream& out, const TaskInfo& obj);


    class rent_task : public virtual ::apache::thrift::TBase {
    public:

        rent_task(const rent_task&);
        rent_task& operator=(const rent_task&);
        rent_task() : rent_wallet(), rent_end(0) {
        }

        virtual ~rent_task() throw();
        std::string rent_wallet;
        std::vector<std::string>  task_ids;
        int64_t rent_end;

        void __set_rent_wallet(const std::string& val);

        void __set_task_ids(const std::vector<std::string> & val);

        void __set_rent_end(const int64_t val);

        bool operator == (const rent_task & rhs) const
        {
            if (!(rent_wallet == rhs.rent_wallet))
                return false;
            if (!(task_ids == rhs.task_ids))
                return false;
            if (!(rent_end == rhs.rent_end))
                return false;
            return true;
        }
        bool operator != (const rent_task &rhs) const {
            return !(*this == rhs);
        }

        bool operator < (const rent_task & ) const;

        uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
        uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

        virtual void printTo(std::ostream& out) const;
    };

    void swap(rent_task &a, rent_task &b);

    std::ostream& operator<<(std::ostream& out, const rent_task& obj);

} // namespace

#endif
