/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef db_task_iptable_TYPES_H
#define db_task_iptable_TYPES_H

#include <iosfwd>
#include "network/protocol/protocol.h"
#include "network/protocol/net_message.h"
#include "util/TToString.h"

namespace dbc {

	class db_task_iptable;

	typedef struct _db_task_iptable__isset {
		_db_task_iptable__isset() : host_ip(false), task_local_ip(false), ssh_port(false), rdp_port(false), custom_port(false), public_ip(false) {}
		bool host_ip : 1;
		bool task_local_ip : 1;
		bool ssh_port : 1;
		bool rdp_port : 1;
		bool custom_port : 1;
		bool public_ip : 1;
	} _db_task_iptable__isset;

	class db_task_iptable : public virtual ::apache::thrift::TBase {
	public:

		db_task_iptable(const db_task_iptable&);
		db_task_iptable& operator=(const db_task_iptable&);
		db_task_iptable() : task_id(), host_ip(), task_local_ip(), ssh_port(), rdp_port(), public_ip() {
		}

		virtual ~db_task_iptable() throw();
		std::string task_id;
		std::string host_ip;
		std::string task_local_ip;
		std::string ssh_port;
		std::string rdp_port;
		std::vector<std::string>  custom_port;
		std::string public_ip;

		_db_task_iptable__isset __isset;

		void __set_task_id(const std::string& val);

		void __set_host_ip(const std::string& val);

		void __set_task_local_ip(const std::string& val);

		void __set_ssh_port(const std::string& val);

		void __set_rdp_port(const std::string& val);

		void __set_custom_port(const std::vector<std::string>& val);

		void __set_public_ip(const std::string& val);

		bool operator == (const db_task_iptable& rhs) const
		{
			if (!(task_id == rhs.task_id))
				return false;
			if (__isset.host_ip != rhs.__isset.host_ip)
				return false;
			else if (__isset.host_ip && !(host_ip == rhs.host_ip))
				return false;
			if (__isset.task_local_ip != rhs.__isset.task_local_ip)
				return false;
			else if (__isset.task_local_ip && !(task_local_ip == rhs.task_local_ip))
				return false;
			if (__isset.ssh_port != rhs.__isset.ssh_port)
				return false;
			else if (__isset.ssh_port && !(ssh_port == rhs.ssh_port))
				return false;
			if (__isset.rdp_port != rhs.__isset.rdp_port)
				return false;
			else if (__isset.rdp_port && !(rdp_port == rhs.rdp_port))
				return false;
			if (__isset.custom_port != rhs.__isset.custom_port)
				return false;
			else if (__isset.custom_port && !(custom_port == rhs.custom_port))
				return false;
			if (__isset.public_ip != rhs.__isset.public_ip)
				return false;
			else if (__isset.public_ip && !(public_ip == rhs.public_ip))
				return false;
			return true;
		}
		bool operator != (const db_task_iptable& rhs) const {
			return !(*this == rhs);
		}

		bool operator < (const db_task_iptable&) const;

		uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
		uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

		virtual void printTo(std::ostream& out) const;
	};

	void swap(db_task_iptable& a, db_task_iptable& b);

	std::ostream& operator<<(std::ostream& out, const db_task_iptable& obj);

} // namespace

#endif