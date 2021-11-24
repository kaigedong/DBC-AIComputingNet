/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "sessionid_db_types.h"
#include <algorithm>
#include <ostream>

namespace dbc {


    rent_sessionid::~rent_sessionid() throw() {
    }


    void rent_sessionid::__set_rent_wallet(const std::string& val) {
        this->rent_wallet = val;
    }

    void rent_sessionid::__set_session_id(const std::string& val) {
        this->session_id = val;
    }
    std::ostream& operator<<(std::ostream& out, const rent_sessionid& obj)
    {
        obj.printTo(out);
        return out;
    }


    uint32_t rent_sessionid::read(::apache::thrift::protocol::TProtocol* iprot) {

        ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
        uint32_t xfer = 0;
        std::string fname;
        ::apache::thrift::protocol::TType ftype;
        int16_t fid;

        xfer += iprot->readStructBegin(fname);

        using ::apache::thrift::protocol::TProtocolException;

        bool isset_rent_wallet = false;
        bool isset_session_id = false;

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
                        if (ftype == ::apache::thrift::protocol::T_STRING) {
                            xfer += iprot->readString(this->session_id);
                            isset_session_id = true;
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
        if (!isset_session_id)
            throw TProtocolException(TProtocolException::INVALID_DATA);
        return xfer;
    }

    uint32_t rent_sessionid::write(::apache::thrift::protocol::TProtocol* oprot) const {
        uint32_t xfer = 0;
        ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
        xfer += oprot->writeStructBegin("rent_sessionid");

        xfer += oprot->writeFieldBegin("rent_wallet", ::apache::thrift::protocol::T_STRING, 1);
        xfer += oprot->writeString(this->rent_wallet);
        xfer += oprot->writeFieldEnd();

        xfer += oprot->writeFieldBegin("session_id", ::apache::thrift::protocol::T_STRING, 2);
        xfer += oprot->writeString(this->session_id);
        xfer += oprot->writeFieldEnd();

        xfer += oprot->writeFieldStop();
        xfer += oprot->writeStructEnd();
        return xfer;
    }

    void swap(rent_sessionid &a, rent_sessionid &b) {
        using ::std::swap;
        swap(a.rent_wallet, b.rent_wallet);
        swap(a.session_id, b.session_id);
    }

    rent_sessionid::rent_sessionid(const rent_sessionid& other0) {
        rent_wallet = other0.rent_wallet;
        session_id = other0.session_id;
    }
    rent_sessionid& rent_sessionid::operator=(const rent_sessionid& other1) {
        rent_wallet = other1.rent_wallet;
        session_id = other1.session_id;
        return *this;
    }
    void rent_sessionid::printTo(std::ostream& out) const {
        using ::apache::thrift::to_string;
        out << "rent_sessionid(";
        out << "rent_wallet=" << to_string(rent_wallet);
        out << ", " << "session_id=" << to_string(session_id);
        out << ")";
    }

} // namespace