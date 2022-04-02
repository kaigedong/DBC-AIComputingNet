/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "taskinfo_types.h"
#include <algorithm>
#include <ostream>

namespace dbc {


    HardwareResource::~HardwareResource() throw() {
    }


    void HardwareResource::__set_gpu_count(const int32_t val) {
        this->gpu_count = val;
        __isset.gpu_count = true;
    }

    void HardwareResource::__set_cpu_cores(const int32_t val) {
        this->cpu_cores = val;
        __isset.cpu_cores = true;
    }

    void HardwareResource::__set_mem_rate(const double val) {
        this->mem_rate = val;
        __isset.mem_rate = true;
    }
    std::ostream& operator<<(std::ostream& out, const HardwareResource& obj)
    {
        obj.printTo(out);
        return out;
    }


    uint32_t HardwareResource::read(::apache::thrift::protocol::TProtocol* iprot) {

        ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
        uint32_t xfer = 0;
        std::string fname;
        ::apache::thrift::protocol::TType ftype;
        int16_t fid;

        xfer += iprot->readStructBegin(fname);

        using ::apache::thrift::protocol::TProtocolException;


        while (true)
        {
            xfer += iprot->readFieldBegin(fname, ftype, fid);
            if (ftype == ::apache::thrift::protocol::T_STOP) {
                break;
            }
            switch (fid)
            {
                case 1:
                    if (ftype == ::apache::thrift::protocol::T_I32) {
                        xfer += iprot->readI32(this->gpu_count);
                        this->__isset.gpu_count = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 2:
                    if (ftype == ::apache::thrift::protocol::T_I32) {
                        xfer += iprot->readI32(this->cpu_cores);
                        this->__isset.cpu_cores = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 3:
                    if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
                        xfer += iprot->readDouble(this->mem_rate);
                        this->__isset.mem_rate = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                default:
                    xfer += iprot->skip(ftype);
                    break;
            }
            xfer += iprot->readFieldEnd();
        }

        xfer += iprot->readStructEnd();

        return xfer;
    }

    uint32_t HardwareResource::write(::apache::thrift::protocol::TProtocol* oprot) const {
        uint32_t xfer = 0;
        ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
        xfer += oprot->writeStructBegin("HardwareResource");

        if (this->__isset.gpu_count) {
            xfer += oprot->writeFieldBegin("gpu_count", ::apache::thrift::protocol::T_I32, 1);
            xfer += oprot->writeI32(this->gpu_count);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.cpu_cores) {
            xfer += oprot->writeFieldBegin("cpu_cores", ::apache::thrift::protocol::T_I32, 2);
            xfer += oprot->writeI32(this->cpu_cores);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.mem_rate) {
            xfer += oprot->writeFieldBegin("mem_rate", ::apache::thrift::protocol::T_DOUBLE, 3);
            xfer += oprot->writeDouble(this->mem_rate);
            xfer += oprot->writeFieldEnd();
        }
        xfer += oprot->writeFieldStop();
        xfer += oprot->writeStructEnd();
        return xfer;
    }

    void swap(HardwareResource &a, HardwareResource &b) {
        using ::std::swap;
        swap(a.gpu_count, b.gpu_count);
        swap(a.cpu_cores, b.cpu_cores);
        swap(a.mem_rate, b.mem_rate);
        swap(a.__isset, b.__isset);
    }

    HardwareResource::HardwareResource(const HardwareResource& other0) {
        gpu_count = other0.gpu_count;
        cpu_cores = other0.cpu_cores;
        mem_rate = other0.mem_rate;
        __isset = other0.__isset;
    }
    HardwareResource& HardwareResource::operator=(const HardwareResource& other1) {
        gpu_count = other1.gpu_count;
        cpu_cores = other1.cpu_cores;
        mem_rate = other1.mem_rate;
        __isset = other1.__isset;
        return *this;
    }
    void HardwareResource::printTo(std::ostream& out) const {
        using ::apache::thrift::to_string;
        out << "HardwareResource(";
        out << "gpu_count="; (__isset.gpu_count ? (out << to_string(gpu_count)) : (out << "<null>"));
        out << ", " << "cpu_cores="; (__isset.cpu_cores ? (out << to_string(cpu_cores)) : (out << "<null>"));
        out << ", " << "mem_rate="; (__isset.mem_rate ? (out << to_string(mem_rate)) : (out << "<null>"));
        out << ")";
    }


    TaskInfo::~TaskInfo() throw() {
    }


    void TaskInfo::__set_task_id(const std::string& val) {
        this->task_id = val;
    }

    void TaskInfo::__set_image_name(const std::string& val) {
        this->image_name = val;
        __isset.image_name = true;
    }

    void TaskInfo::__set_login_password(const std::string& val) {
        this->login_password = val;
        __isset.login_password = true;
    }

    void TaskInfo::__set_ssh_port(const std::string& val) {
        this->ssh_port = val;
        __isset.ssh_port = true;
    }

    void TaskInfo::__set_status(const int32_t val) {
        this->status = val;
        __isset.status = true;
    }

    void TaskInfo::__set_operation(const int32_t val) {
        this->operation = val;
        __isset.operation = true;
    }

    void TaskInfo::__set_create_time(const int64_t val) {
        this->create_time = val;
        __isset.create_time = true;
    }

    void TaskInfo::__set_last_start_time(const int64_t val) {
        this->last_start_time = val;
        __isset.last_start_time = true;
    }

    void TaskInfo::__set_last_stop_time(const int64_t val) {
        this->last_stop_time = val;
        __isset.last_stop_time = true;
    }

    void TaskInfo::__set_hardware_resource(const HardwareResource& val) {
        this->hardware_resource = val;
    }

    void TaskInfo::__set_vm_xml(const std::string& val) {
        this->vm_xml = val;
        __isset.vm_xml = true;
    }

    void TaskInfo::__set_vm_xml_url(const std::string& val) {
        this->vm_xml_url = val;
        __isset.vm_xml_url = true;
    }

    void TaskInfo::__set_data_file_name(const std::string& val) {
        this->data_file_name = val;
        __isset.data_file_name = true;
    }

    void TaskInfo::__set_operation_system(const std::string& val) {
        this->operation_system = val;
        __isset.operation_system = true;
    }

    void TaskInfo::__set_bios_mode(const std::string& val) {
        this->bios_mode = val;
        __isset.bios_mode = true;
    }

    void TaskInfo::__set_rdp_port(const std::string& val) {
        this->rdp_port = val;
        __isset.rdp_port = true;
    }

    void TaskInfo::__set_custom_port(const std::vector<std::string> & val) {
        this->custom_port = val;
        __isset.custom_port = true;
    }

    void TaskInfo::__set_multicast(const std::vector<std::string> & val) {
        this->multicast = val;
        __isset.multicast = true;
    }

    void TaskInfo::__set_custom_image_name(const std::string& val) {
        this->custom_image_name = val;
        __isset.custom_image_name = true;
    }

    void TaskInfo::__set_network_name(const std::string& val) {
        this->network_name = val;
        __isset.network_name = true;
    }
    std::ostream& operator<<(std::ostream& out, const TaskInfo& obj)
    {
        obj.printTo(out);
        return out;
    }


    uint32_t TaskInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

        ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
        uint32_t xfer = 0;
        std::string fname;
        ::apache::thrift::protocol::TType ftype;
        int16_t fid;

        xfer += iprot->readStructBegin(fname);

        using ::apache::thrift::protocol::TProtocolException;

        bool isset_task_id = false;

        while (true)
        {
            xfer += iprot->readFieldBegin(fname, ftype, fid);
            if (ftype == ::apache::thrift::protocol::T_STOP) {
                break;
            }
            switch (fid)
            {
                case 1:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->task_id);
                        isset_task_id = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 2:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->image_name);
                        this->__isset.image_name = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 3:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->login_password);
                        this->__isset.login_password = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 4:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->ssh_port);
                        this->__isset.ssh_port = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 5:
                    if (ftype == ::apache::thrift::protocol::T_I32) {
                        xfer += iprot->readI32(this->status);
                        this->__isset.status = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 6:
                    if (ftype == ::apache::thrift::protocol::T_I32) {
                        xfer += iprot->readI32(this->operation);
                        this->__isset.operation = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 7:
                    if (ftype == ::apache::thrift::protocol::T_I64) {
                        xfer += iprot->readI64(this->create_time);
                        this->__isset.create_time = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 8:
                    if (ftype == ::apache::thrift::protocol::T_I64) {
                        xfer += iprot->readI64(this->last_start_time);
                        this->__isset.last_start_time = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 9:
                    if (ftype == ::apache::thrift::protocol::T_I64) {
                        xfer += iprot->readI64(this->last_stop_time);
                        this->__isset.last_stop_time = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 10:
                    if (ftype == ::apache::thrift::protocol::T_STRUCT) {
                        xfer += this->hardware_resource.read(iprot);
                        this->__isset.hardware_resource = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 11:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->vm_xml);
                        this->__isset.vm_xml = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 12:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->vm_xml_url);
                        this->__isset.vm_xml_url = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 13:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->data_file_name);
                        this->__isset.data_file_name = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 14:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->operation_system);
                        this->__isset.operation_system = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 15:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->bios_mode);
                        this->__isset.bios_mode = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 16:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->rdp_port);
                        this->__isset.rdp_port = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 17:
                    if (ftype == ::apache::thrift::protocol::T_LIST) {
                        {
                            this->custom_port.clear();
                            uint32_t _size2;
                            ::apache::thrift::protocol::TType _etype5;
                            xfer += iprot->readListBegin(_etype5, _size2);
                            this->custom_port.resize(_size2);
                            uint32_t _i6;
                            for (_i6 = 0; _i6 < _size2; ++_i6)
                            {
                                xfer += iprot->readString(this->custom_port[_i6]);
                            }
                            xfer += iprot->readListEnd();
                        }
                        this->__isset.custom_port = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 18:
                    if (ftype == ::apache::thrift::protocol::T_LIST) {
                        {
                            this->multicast.clear();
                            uint32_t _size7;
                            ::apache::thrift::protocol::TType _etype10;
                            xfer += iprot->readListBegin(_etype10, _size7);
                            this->multicast.resize(_size7);
                            uint32_t _i11;
                            for (_i11 = 0; _i11 < _size7; ++_i11)
                            {
                                xfer += iprot->readString(this->multicast[_i11]);
                            }
                            xfer += iprot->readListEnd();
                        }
                        this->__isset.multicast = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 19:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->custom_image_name);
                        this->__isset.custom_image_name = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 24:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->network_name);
                        this->__isset.network_name = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                default:
                    xfer += iprot->skip(ftype);
                    break;
            }
            xfer += iprot->readFieldEnd();
        }

        xfer += iprot->readStructEnd();

        if (!isset_task_id)
            throw TProtocolException(TProtocolException::INVALID_DATA);
        return xfer;
    }

    uint32_t TaskInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
        uint32_t xfer = 0;
        ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
        xfer += oprot->writeStructBegin("TaskInfo");

        xfer += oprot->writeFieldBegin("task_id", ::apache::thrift::protocol::T_STRING, 1);
        xfer += oprot->writeString(this->task_id);
        xfer += oprot->writeFieldEnd();

        if (this->__isset.image_name) {
            xfer += oprot->writeFieldBegin("image_name", ::apache::thrift::protocol::T_STRING, 2);
            xfer += oprot->writeString(this->image_name);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.login_password) {
            xfer += oprot->writeFieldBegin("login_password", ::apache::thrift::protocol::T_STRING, 3);
            xfer += oprot->writeString(this->login_password);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.ssh_port) {
            xfer += oprot->writeFieldBegin("ssh_port", ::apache::thrift::protocol::T_STRING, 4);
            xfer += oprot->writeString(this->ssh_port);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.status) {
            xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 5);
            xfer += oprot->writeI32(this->status);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.operation) {
            xfer += oprot->writeFieldBegin("operation", ::apache::thrift::protocol::T_I32, 6);
            xfer += oprot->writeI32(this->operation);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.create_time) {
            xfer += oprot->writeFieldBegin("create_time", ::apache::thrift::protocol::T_I64, 7);
            xfer += oprot->writeI64(this->create_time);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.last_start_time) {
            xfer += oprot->writeFieldBegin("last_start_time", ::apache::thrift::protocol::T_I64, 8);
            xfer += oprot->writeI64(this->last_start_time);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.last_stop_time) {
            xfer += oprot->writeFieldBegin("last_stop_time", ::apache::thrift::protocol::T_I64, 9);
            xfer += oprot->writeI64(this->last_stop_time);
            xfer += oprot->writeFieldEnd();
        }
        xfer += oprot->writeFieldBegin("hardware_resource", ::apache::thrift::protocol::T_STRUCT, 10);
        xfer += this->hardware_resource.write(oprot);
        xfer += oprot->writeFieldEnd();

        if (this->__isset.vm_xml) {
            xfer += oprot->writeFieldBegin("vm_xml", ::apache::thrift::protocol::T_STRING, 11);
            xfer += oprot->writeString(this->vm_xml);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.vm_xml_url) {
            xfer += oprot->writeFieldBegin("vm_xml_url", ::apache::thrift::protocol::T_STRING, 12);
            xfer += oprot->writeString(this->vm_xml_url);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.data_file_name) {
            xfer += oprot->writeFieldBegin("data_file_name", ::apache::thrift::protocol::T_STRING, 13);
            xfer += oprot->writeString(this->data_file_name);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.operation_system) {
            xfer += oprot->writeFieldBegin("operation_system", ::apache::thrift::protocol::T_STRING, 14);
            xfer += oprot->writeString(this->operation_system);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.bios_mode) {
            xfer += oprot->writeFieldBegin("bios_mode", ::apache::thrift::protocol::T_STRING, 15);
            xfer += oprot->writeString(this->bios_mode);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.rdp_port) {
            xfer += oprot->writeFieldBegin("rdp_port", ::apache::thrift::protocol::T_STRING, 16);
            xfer += oprot->writeString(this->rdp_port);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.custom_port) {
            xfer += oprot->writeFieldBegin("custom_port", ::apache::thrift::protocol::T_LIST, 17);
            {
                xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->custom_port.size()));
                std::vector<std::string> ::const_iterator _iter12;
                for (_iter12 = this->custom_port.begin(); _iter12 != this->custom_port.end(); ++_iter12)
                {
                    xfer += oprot->writeString((*_iter12));
                }
                xfer += oprot->writeListEnd();
            }
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.multicast) {
            xfer += oprot->writeFieldBegin("multicast", ::apache::thrift::protocol::T_LIST, 18);
            {
                xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->multicast.size()));
                std::vector<std::string> ::const_iterator _iter13;
                for (_iter13 = this->multicast.begin(); _iter13 != this->multicast.end(); ++_iter13)
                {
                    xfer += oprot->writeString((*_iter13));
                }
                xfer += oprot->writeListEnd();
            }
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.custom_image_name) {
            xfer += oprot->writeFieldBegin("custom_image_name", ::apache::thrift::protocol::T_STRING, 19);
            xfer += oprot->writeString(this->custom_image_name);
            xfer += oprot->writeFieldEnd();
        }
        if (this->__isset.network_name) {
            xfer += oprot->writeFieldBegin("network_name", ::apache::thrift::protocol::T_STRING, 24);
            xfer += oprot->writeString(this->network_name);
            xfer += oprot->writeFieldEnd();
        }
        xfer += oprot->writeFieldStop();
        xfer += oprot->writeStructEnd();
        return xfer;
    }

    void swap(TaskInfo &a, TaskInfo &b) {
        using ::std::swap;
        swap(a.task_id, b.task_id);
        swap(a.image_name, b.image_name);
        swap(a.login_password, b.login_password);
        swap(a.ssh_port, b.ssh_port);
        swap(a.status, b.status);
        swap(a.operation, b.operation);
        swap(a.create_time, b.create_time);
        swap(a.last_start_time, b.last_start_time);
        swap(a.last_stop_time, b.last_stop_time);
        swap(a.hardware_resource, b.hardware_resource);
        swap(a.vm_xml, b.vm_xml);
        swap(a.vm_xml_url, b.vm_xml_url);
        swap(a.data_file_name, b.data_file_name);
        swap(a.operation_system, b.operation_system);
        swap(a.bios_mode, b.bios_mode);
        swap(a.rdp_port, b.rdp_port);
        swap(a.custom_port, b.custom_port);
        swap(a.multicast, b.multicast);
        swap(a.custom_image_name, b.custom_image_name);
        swap(a.network_name, b.network_name);
        swap(a.__isset, b.__isset);
    }

    TaskInfo::TaskInfo(const TaskInfo& other14) {
        task_id = other14.task_id;
        image_name = other14.image_name;
        login_password = other14.login_password;
        ssh_port = other14.ssh_port;
        status = other14.status;
        operation = other14.operation;
        create_time = other14.create_time;
        last_start_time = other14.last_start_time;
        last_stop_time = other14.last_stop_time;
        hardware_resource = other14.hardware_resource;
        vm_xml = other14.vm_xml;
        vm_xml_url = other14.vm_xml_url;
        data_file_name = other14.data_file_name;
        operation_system = other14.operation_system;
        bios_mode = other14.bios_mode;
        rdp_port = other14.rdp_port;
        custom_port = other14.custom_port;
        multicast = other14.multicast;
        custom_image_name = other14.custom_image_name;
        network_name = other14.network_name;
        __isset = other14.__isset;
    }
    TaskInfo& TaskInfo::operator=(const TaskInfo& other15) {
        task_id = other15.task_id;
        image_name = other15.image_name;
        login_password = other15.login_password;
        ssh_port = other15.ssh_port;
        status = other15.status;
        operation = other15.operation;
        create_time = other15.create_time;
        last_start_time = other15.last_start_time;
        last_stop_time = other15.last_stop_time;
        hardware_resource = other15.hardware_resource;
        vm_xml = other15.vm_xml;
        vm_xml_url = other15.vm_xml_url;
        data_file_name = other15.data_file_name;
        operation_system = other15.operation_system;
        bios_mode = other15.bios_mode;
        rdp_port = other15.rdp_port;
        custom_port = other15.custom_port;
        multicast = other15.multicast;
        custom_image_name = other15.custom_image_name;
        network_name = other15.network_name;
        __isset = other15.__isset;
        return *this;
    }
    void TaskInfo::printTo(std::ostream& out) const {
        using ::apache::thrift::to_string;
        out << "TaskInfo(";
        out << "task_id=" << to_string(task_id);
        out << ", " << "image_name="; (__isset.image_name ? (out << to_string(image_name)) : (out << "<null>"));
        out << ", " << "login_password="; (__isset.login_password ? (out << to_string(login_password)) : (out << "<null>"));
        out << ", " << "ssh_port="; (__isset.ssh_port ? (out << to_string(ssh_port)) : (out << "<null>"));
        out << ", " << "status="; (__isset.status ? (out << to_string(status)) : (out << "<null>"));
        out << ", " << "operation="; (__isset.operation ? (out << to_string(operation)) : (out << "<null>"));
        out << ", " << "create_time="; (__isset.create_time ? (out << to_string(create_time)) : (out << "<null>"));
        out << ", " << "last_start_time="; (__isset.last_start_time ? (out << to_string(last_start_time)) : (out << "<null>"));
        out << ", " << "last_stop_time="; (__isset.last_stop_time ? (out << to_string(last_stop_time)) : (out << "<null>"));
        out << ", " << "hardware_resource=" << to_string(hardware_resource);
        out << ", " << "vm_xml="; (__isset.vm_xml ? (out << to_string(vm_xml)) : (out << "<null>"));
        out << ", " << "vm_xml_url="; (__isset.vm_xml_url ? (out << to_string(vm_xml_url)) : (out << "<null>"));
        out << ", " << "data_file_name="; (__isset.data_file_name ? (out << to_string(data_file_name)) : (out << "<null>"));
        out << ", " << "operation_system="; (__isset.operation_system ? (out << to_string(operation_system)) : (out << "<null>"));
        out << ", " << "bios_mode="; (__isset.bios_mode ? (out << to_string(bios_mode)) : (out << "<null>"));
        out << ", " << "rdp_port="; (__isset.rdp_port ? (out << to_string(rdp_port)) : (out << "<null>"));
        out << ", " << "custom_port="; (__isset.custom_port ? (out << to_string(custom_port)) : (out << "<null>"));
        out << ", " << "multicast="; (__isset.multicast ? (out << to_string(multicast)) : (out << "<null>"));
        out << ", " << "custom_image_name="; (__isset.custom_image_name ? (out << to_string(custom_image_name)) : (out << "<null>"));
        out << ", " << "network_name="; (__isset.network_name ? (out << to_string(network_name)) : (out << "<null>"));
        out << ")";
    }


    rent_task::~rent_task() throw() {
    }


    void rent_task::__set_rent_wallet(const std::string& val) {
        this->rent_wallet = val;
    }

    void rent_task::__set_task_ids(const std::vector<std::string> & val) {
        this->task_ids = val;
    }

    void rent_task::__set_rent_end(const int64_t val) {
        this->rent_end = val;
    }
    std::ostream& operator<<(std::ostream& out, const rent_task& obj)
    {
        obj.printTo(out);
        return out;
    }


    uint32_t rent_task::read(::apache::thrift::protocol::TProtocol* iprot) {

        ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
        uint32_t xfer = 0;
        std::string fname;
        ::apache::thrift::protocol::TType ftype;
        int16_t fid;

        xfer += iprot->readStructBegin(fname);

        using ::apache::thrift::protocol::TProtocolException;

        bool isset_rent_wallet = false;
        bool isset_task_ids = false;
        bool isset_rent_end = false;

        while (true)
        {
            xfer += iprot->readFieldBegin(fname, ftype, fid);
            if (ftype == ::apache::thrift::protocol::T_STOP) {
                break;
            }
            switch (fid)
            {
                case 1:
                    if (ftype == ::apache::thrift::protocol::T_STRING) {
                        xfer += iprot->readString(this->rent_wallet);
                        isset_rent_wallet = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 2:
                    if (ftype == ::apache::thrift::protocol::T_LIST) {
                        {
                            this->task_ids.clear();
                            uint32_t _size16;
                            ::apache::thrift::protocol::TType _etype19;
                            xfer += iprot->readListBegin(_etype19, _size16);
                            this->task_ids.resize(_size16);
                            uint32_t _i20;
                            for (_i20 = 0; _i20 < _size16; ++_i20)
                            {
                                xfer += iprot->readString(this->task_ids[_i20]);
                            }
                            xfer += iprot->readListEnd();
                        }
                        isset_task_ids = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                case 3:
                    if (ftype == ::apache::thrift::protocol::T_I64) {
                        xfer += iprot->readI64(this->rent_end);
                        isset_rent_end = true;
                    } else {
                        xfer += iprot->skip(ftype);
                    }
                    break;
                default:
                    xfer += iprot->skip(ftype);
                    break;
            }
            xfer += iprot->readFieldEnd();
        }

        xfer += iprot->readStructEnd();

        if (!isset_rent_wallet)
            throw TProtocolException(TProtocolException::INVALID_DATA);
        if (!isset_task_ids)
            throw TProtocolException(TProtocolException::INVALID_DATA);
        if (!isset_rent_end)
            throw TProtocolException(TProtocolException::INVALID_DATA);
        return xfer;
    }

    uint32_t rent_task::write(::apache::thrift::protocol::TProtocol* oprot) const {
        uint32_t xfer = 0;
        ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
        xfer += oprot->writeStructBegin("rent_task");

        xfer += oprot->writeFieldBegin("rent_wallet", ::apache::thrift::protocol::T_STRING, 1);
        xfer += oprot->writeString(this->rent_wallet);
        xfer += oprot->writeFieldEnd();

        xfer += oprot->writeFieldBegin("task_ids", ::apache::thrift::protocol::T_LIST, 2);
        {
            xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->task_ids.size()));
            std::vector<std::string> ::const_iterator _iter21;
            for (_iter21 = this->task_ids.begin(); _iter21 != this->task_ids.end(); ++_iter21)
            {
                xfer += oprot->writeString((*_iter21));
            }
            xfer += oprot->writeListEnd();
        }
        xfer += oprot->writeFieldEnd();

        xfer += oprot->writeFieldBegin("rent_end", ::apache::thrift::protocol::T_I64, 3);
        xfer += oprot->writeI64(this->rent_end);
        xfer += oprot->writeFieldEnd();

        xfer += oprot->writeFieldStop();
        xfer += oprot->writeStructEnd();
        return xfer;
    }

    void swap(rent_task &a, rent_task &b) {
        using ::std::swap;
        swap(a.rent_wallet, b.rent_wallet);
        swap(a.task_ids, b.task_ids);
        swap(a.rent_end, b.rent_end);
    }

    rent_task::rent_task(const rent_task& other22) {
        rent_wallet = other22.rent_wallet;
        task_ids = other22.task_ids;
        rent_end = other22.rent_end;
    }
    rent_task& rent_task::operator=(const rent_task& other23) {
        rent_wallet = other23.rent_wallet;
        task_ids = other23.task_ids;
        rent_end = other23.rent_end;
        return *this;
    }
    void rent_task::printTo(std::ostream& out) const {
        using ::apache::thrift::to_string;
        out << "rent_task(";
        out << "rent_wallet=" << to_string(rent_wallet);
        out << ", " << "task_ids=" << to_string(task_ids);
        out << ", " << "rent_end=" << to_string(rent_end);
        out << ")";
    }

} // namespace