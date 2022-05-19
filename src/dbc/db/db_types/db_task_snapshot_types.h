/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef db_task_snapshot_TYPES_H
#define db_task_snapshot_TYPES_H

#include <iosfwd>
#include "network/protocol/protocol.h"
#include "network/protocol/net_message.h"
#include "util/TToString.h"


namespace dbc {

	class snapshot_info;

	class db_snapshot_info;


	class snapshot_info : public virtual ::apache::thrift::TBase {
	public:

		snapshot_info(const snapshot_info&);
		snapshot_info& operator=(const snapshot_info&);
		snapshot_info() : name(), file(), create_time(0), desc() {
		}

		virtual ~snapshot_info() throw();
		std::string name;
		std::string file;
		int64_t create_time;
		std::string desc;

		void __set_name(const std::string& val);

		void __set_file(const std::string& val);

		void __set_create_time(const int64_t val);

		void __set_desc(const std::string& val);

		bool operator == (const snapshot_info& rhs) const
		{
			if (!(name == rhs.name))
				return false;
			if (!(file == rhs.file))
				return false;
			if (!(create_time == rhs.create_time))
				return false;
			if (!(desc == rhs.desc))
				return false;
			return true;
		}
		bool operator != (const snapshot_info& rhs) const {
			return !(*this == rhs);
		}

		bool operator < (const snapshot_info&) const;

		uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
		uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

		virtual void printTo(std::ostream& out) const;
	};

	void swap(snapshot_info& a, snapshot_info& b);

	std::ostream& operator<<(std::ostream& out, const snapshot_info& obj);


	class db_snapshot_info : public virtual ::apache::thrift::TBase {
	public:

		db_snapshot_info(const db_snapshot_info&);
		db_snapshot_info& operator=(const db_snapshot_info&);
		db_snapshot_info() : task_id() {
		}

		virtual ~db_snapshot_info() throw();
		std::string task_id;
		std::vector<snapshot_info>  snapshots;

		void __set_task_id(const std::string& val);

		void __set_snapshots(const std::vector<snapshot_info>& val);

		bool operator == (const db_snapshot_info& rhs) const
		{
			if (!(task_id == rhs.task_id))
				return false;
			if (!(snapshots == rhs.snapshots))
				return false;
			return true;
		}
		bool operator != (const db_snapshot_info& rhs) const {
			return !(*this == rhs);
		}

		bool operator < (const db_snapshot_info&) const;

		uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
		uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

		virtual void printTo(std::ostream& out) const;
	};

	void swap(db_snapshot_info& a, db_snapshot_info& b);

	std::ostream& operator<<(std::ostream& out, const db_snapshot_info& obj);

} // namespace

#endif