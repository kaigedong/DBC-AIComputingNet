/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef sessionid_db_TYPES_H
#define sessionid_db_TYPES_H

#include <iosfwd>
#include "network/protocol/protocol.h"
#include "network/protocol/service_message.h"
#include "util/TToString.h"

namespace dbc {

class owner_sessionid;


class owner_sessionid : public virtual ::apache::thrift::TBase {
 public:

  owner_sessionid(const owner_sessionid&);
  owner_sessionid& operator=(const owner_sessionid&);
  owner_sessionid() : wallet(), session_id() {
  }

  virtual ~owner_sessionid() throw();
  std::string wallet;
  std::string session_id;

  void __set_wallet(const std::string& val);

  void __set_session_id(const std::string& val);

  bool operator == (const owner_sessionid & rhs) const
  {
    if (!(wallet == rhs.wallet))
      return false;
    if (!(session_id == rhs.session_id))
      return false;
    return true;
  }
  bool operator != (const owner_sessionid &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const owner_sessionid & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(owner_sessionid &a, owner_sessionid &b);

std::ostream& operator<<(std::ostream& out, const owner_sessionid& obj);

} // namespace

#endif