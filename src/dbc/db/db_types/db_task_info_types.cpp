/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "db_task_info_types.h"
#include <algorithm>
#include <ostream>

namespace dbc {


	db_task_info::~db_task_info() throw() {
	}


	void db_task_info::__set_task_id(const std::string& val) {
		this->task_id = val;
	}

	void db_task_info::__set_image_name(const std::string& val) {
		this->image_name = val;
		__isset.image_name = true;
	}

	void db_task_info::__set_login_password(const std::string& val) {
		this->login_password = val;
		__isset.login_password = true;
	}

	void db_task_info::__set_ssh_port(const std::string& val) {
		this->ssh_port = val;
		__isset.ssh_port = true;
	}

	void db_task_info::__set_create_time(const int64_t val) {
		this->create_time = val;
		__isset.create_time = true;
	}

	void db_task_info::__set_operation_system(const std::string& val) {
		this->operation_system = val;
		__isset.operation_system = true;
	}

	void db_task_info::__set_bios_mode(const std::string& val) {
		this->bios_mode = val;
		__isset.bios_mode = true;
	}

	void db_task_info::__set_rdp_port(const std::string& val) {
		this->rdp_port = val;
		__isset.rdp_port = true;
	}

	void db_task_info::__set_custom_port(const std::vector<std::string>& val) {
		this->custom_port = val;
		__isset.custom_port = true;
	}

	void db_task_info::__set_multicast(const std::vector<std::string>& val) {
		this->multicast = val;
		__isset.multicast = true;
	}

	void db_task_info::__set_desc(const std::string& val) {
		this->desc = val;
		__isset.desc = true;
	}

	void db_task_info::__set_vda_rootbackfile(const std::string& val) {
		this->vda_rootbackfile = val;
		__isset.vda_rootbackfile = true;
	}

	void db_task_info::__set_network_name(const std::string& val) {
		this->network_name = val;
		__isset.network_name = true;
	}

	void db_task_info::__set_public_ip(const std::string& val) {
		this->public_ip = val;
		__isset.public_ip = true;
	}

	void db_task_info::__set_nwfilter(const std::vector<std::string>& val) {
		this->nwfilter = val;
		__isset.nwfilter = true;
	}

	void db_task_info::__set_login_username(const std::string& val) {
		this->login_username = val;
		__isset.login_username = true;
	}
	std::ostream& operator<<(std::ostream& out, const db_task_info& obj)
	{
		obj.printTo(out);
		return out;
	}


	uint32_t db_task_info::read(::apache::thrift::protocol::TProtocol* iprot) {

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
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 2:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->image_name);
					this->__isset.image_name = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 3:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->login_password);
					this->__isset.login_password = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 4:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->ssh_port);
					this->__isset.ssh_port = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 7:
				if (ftype == ::apache::thrift::protocol::T_I64) {
					xfer += iprot->readI64(this->create_time);
					this->__isset.create_time = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 14:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->operation_system);
					this->__isset.operation_system = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 15:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->bios_mode);
					this->__isset.bios_mode = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 16:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->rdp_port);
					this->__isset.rdp_port = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 17:
				if (ftype == ::apache::thrift::protocol::T_LIST) {
					{
						this->custom_port.clear();
						uint32_t _size0;
						::apache::thrift::protocol::TType _etype3;
						xfer += iprot->readListBegin(_etype3, _size0);
						this->custom_port.resize(_size0);
						uint32_t _i4;
						for (_i4 = 0; _i4 < _size0; ++_i4)
						{
							xfer += iprot->readString(this->custom_port[_i4]);
						}
						xfer += iprot->readListEnd();
					}
					this->__isset.custom_port = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 18:
				if (ftype == ::apache::thrift::protocol::T_LIST) {
					{
						this->multicast.clear();
						uint32_t _size5;
						::apache::thrift::protocol::TType _etype8;
						xfer += iprot->readListBegin(_etype8, _size5);
						this->multicast.resize(_size5);
						uint32_t _i9;
						for (_i9 = 0; _i9 < _size5; ++_i9)
						{
							xfer += iprot->readString(this->multicast[_i9]);
						}
						xfer += iprot->readListEnd();
					}
					this->__isset.multicast = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 21:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->desc);
					this->__isset.desc = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 22:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->vda_rootbackfile);
					this->__isset.vda_rootbackfile = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 24:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->network_name);
					this->__isset.network_name = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 30:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->public_ip);
					this->__isset.public_ip = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 31:
				if (ftype == ::apache::thrift::protocol::T_LIST) {
					{
						this->nwfilter.clear();
						uint32_t _size10;
						::apache::thrift::protocol::TType _etype13;
						xfer += iprot->readListBegin(_etype13, _size10);
						this->nwfilter.resize(_size10);
						uint32_t _i14;
						for (_i14 = 0; _i14 < _size10; ++_i14)
						{
							xfer += iprot->readString(this->nwfilter[_i14]);
						}
						xfer += iprot->readListEnd();
					}
					this->__isset.nwfilter = true;
				}
				else {
					xfer += iprot->skip(ftype);
				}
				break;
			case 35:
				if (ftype == ::apache::thrift::protocol::T_STRING) {
					xfer += iprot->readString(this->login_username);
					this->__isset.login_username = true;
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

	uint32_t db_task_info::write(::apache::thrift::protocol::TProtocol* oprot) const {
		uint32_t xfer = 0;
		::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
		xfer += oprot->writeStructBegin("db_task_info");

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
		if (this->__isset.create_time) {
			xfer += oprot->writeFieldBegin("create_time", ::apache::thrift::protocol::T_I64, 7);
			xfer += oprot->writeI64(this->create_time);
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
				std::vector<std::string> ::const_iterator _iter15;
				for (_iter15 = this->custom_port.begin(); _iter15 != this->custom_port.end(); ++_iter15)
				{
					xfer += oprot->writeString((*_iter15));
				}
				xfer += oprot->writeListEnd();
			}
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.multicast) {
			xfer += oprot->writeFieldBegin("multicast", ::apache::thrift::protocol::T_LIST, 18);
			{
				xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->multicast.size()));
				std::vector<std::string> ::const_iterator _iter16;
				for (_iter16 = this->multicast.begin(); _iter16 != this->multicast.end(); ++_iter16)
				{
					xfer += oprot->writeString((*_iter16));
				}
				xfer += oprot->writeListEnd();
			}
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.desc) {
			xfer += oprot->writeFieldBegin("desc", ::apache::thrift::protocol::T_STRING, 21);
			xfer += oprot->writeString(this->desc);
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.vda_rootbackfile) {
			xfer += oprot->writeFieldBegin("vda_rootbackfile", ::apache::thrift::protocol::T_STRING, 22);
			xfer += oprot->writeString(this->vda_rootbackfile);
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.network_name) {
			xfer += oprot->writeFieldBegin("network_name", ::apache::thrift::protocol::T_STRING, 24);
			xfer += oprot->writeString(this->network_name);
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.public_ip) {
			xfer += oprot->writeFieldBegin("public_ip", ::apache::thrift::protocol::T_STRING, 30);
			xfer += oprot->writeString(this->public_ip);
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.nwfilter) {
			xfer += oprot->writeFieldBegin("nwfilter", ::apache::thrift::protocol::T_LIST, 31);
			{
				xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->nwfilter.size()));
				std::vector<std::string> ::const_iterator _iter17;
				for (_iter17 = this->nwfilter.begin(); _iter17 != this->nwfilter.end(); ++_iter17)
				{
					xfer += oprot->writeString((*_iter17));
				}
				xfer += oprot->writeListEnd();
			}
			xfer += oprot->writeFieldEnd();
		}
		if (this->__isset.login_username) {
			xfer += oprot->writeFieldBegin("login_username", ::apache::thrift::protocol::T_STRING, 35);
			xfer += oprot->writeString(this->login_username);
			xfer += oprot->writeFieldEnd();
		}
		xfer += oprot->writeFieldStop();
		xfer += oprot->writeStructEnd();
		return xfer;
	}

	void swap(db_task_info& a, db_task_info& b) {
		using ::std::swap;
		swap(a.task_id, b.task_id);
		swap(a.image_name, b.image_name);
		swap(a.login_password, b.login_password);
		swap(a.ssh_port, b.ssh_port);
		swap(a.create_time, b.create_time);
		swap(a.operation_system, b.operation_system);
		swap(a.bios_mode, b.bios_mode);
		swap(a.rdp_port, b.rdp_port);
		swap(a.custom_port, b.custom_port);
		swap(a.multicast, b.multicast);
		swap(a.desc, b.desc);
		swap(a.vda_rootbackfile, b.vda_rootbackfile);
		swap(a.network_name, b.network_name);
		swap(a.public_ip, b.public_ip);
		swap(a.nwfilter, b.nwfilter);
		swap(a.login_username, b.login_username);
		swap(a.__isset, b.__isset);
	}

	db_task_info::db_task_info(const db_task_info& other18) {
		task_id = other18.task_id;
		image_name = other18.image_name;
		login_password = other18.login_password;
		ssh_port = other18.ssh_port;
		create_time = other18.create_time;
		operation_system = other18.operation_system;
		bios_mode = other18.bios_mode;
		rdp_port = other18.rdp_port;
		custom_port = other18.custom_port;
		multicast = other18.multicast;
		desc = other18.desc;
		vda_rootbackfile = other18.vda_rootbackfile;
		network_name = other18.network_name;
		public_ip = other18.public_ip;
		nwfilter = other18.nwfilter;
		login_username = other18.login_username;
		__isset = other18.__isset;
	}
	db_task_info& db_task_info::operator=(const db_task_info& other19) {
		task_id = other19.task_id;
		image_name = other19.image_name;
		login_password = other19.login_password;
		ssh_port = other19.ssh_port;
		create_time = other19.create_time;
		operation_system = other19.operation_system;
		bios_mode = other19.bios_mode;
		rdp_port = other19.rdp_port;
		custom_port = other19.custom_port;
		multicast = other19.multicast;
		desc = other19.desc;
		vda_rootbackfile = other19.vda_rootbackfile;
		network_name = other19.network_name;
		public_ip = other19.public_ip;
		nwfilter = other19.nwfilter;
		login_username = other19.login_username;
		__isset = other19.__isset;
		return *this;
	}
	void db_task_info::printTo(std::ostream& out) const {
		using ::apache::thrift::to_string;
		out << "db_task_info(";
		out << "task_id=" << to_string(task_id);
		out << ", " << "image_name="; (__isset.image_name ? (out << to_string(image_name)) : (out << "<null>"));
		out << ", " << "login_password="; (__isset.login_password ? (out << to_string(login_password)) : (out << "<null>"));
		out << ", " << "ssh_port="; (__isset.ssh_port ? (out << to_string(ssh_port)) : (out << "<null>"));
		out << ", " << "create_time="; (__isset.create_time ? (out << to_string(create_time)) : (out << "<null>"));
		out << ", " << "operation_system="; (__isset.operation_system ? (out << to_string(operation_system)) : (out << "<null>"));
		out << ", " << "bios_mode="; (__isset.bios_mode ? (out << to_string(bios_mode)) : (out << "<null>"));
		out << ", " << "rdp_port="; (__isset.rdp_port ? (out << to_string(rdp_port)) : (out << "<null>"));
		out << ", " << "custom_port="; (__isset.custom_port ? (out << to_string(custom_port)) : (out << "<null>"));
		out << ", " << "multicast="; (__isset.multicast ? (out << to_string(multicast)) : (out << "<null>"));
		out << ", " << "desc="; (__isset.desc ? (out << to_string(desc)) : (out << "<null>"));
		out << ", " << "vda_rootbackfile="; (__isset.vda_rootbackfile ? (out << to_string(vda_rootbackfile)) : (out << "<null>"));
		out << ", " << "network_name="; (__isset.network_name ? (out << to_string(network_name)) : (out << "<null>"));
		out << ", " << "public_ip="; (__isset.public_ip ? (out << to_string(public_ip)) : (out << "<null>"));
		out << ", " << "nwfilter="; (__isset.nwfilter ? (out << to_string(nwfilter)) : (out << "<null>"));
		out << ", " << "login_username="; (__isset.login_username ? (out << to_string(login_username)) : (out << "<null>"));
		out << ")";
	}

} // namespace