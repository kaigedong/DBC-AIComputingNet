/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "db_wallet_renttask_types.h"
#include <algorithm>
#include <ostream>

namespace dbc {


    db_wallet_renttask::~db_wallet_renttask() throw() {
    }


    void db_wallet_renttask::__set_rent_wallet(const std::string& val) {
        this->rent_wallet = val;
    }

    void db_wallet_renttask::__set_task_ids(const std::vector<std::string>& val) {
        this->task_ids = val;
    }

    void db_wallet_renttask::__set_rent_end(const int64_t val) {
        this->rent_end = val;
    }
    std::ostream& operator<<(std::ostream& out, const db_wallet_renttask& obj)
    {
        obj.printTo(out);
        return out;
    }


    uint32_t db_wallet_renttask::read(::apache::thrift::protocol::TProtocol* iprot) {

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
                }
                else {
                    xfer += iprot->skip(ftype);
                }
                break;
            case 2:
                if (ftype == ::apache::thrift::protocol::T_LIST) {
                    {
                        this->task_ids.clear();
                        uint32_t _size0;
                        ::apache::thrift::protocol::TType _etype3;
                        xfer += iprot->readListBegin(_etype3, _size0);
                        this->task_ids.resize(_size0);
                        uint32_t _i4;
                        for (_i4 = 0; _i4 < _size0; ++_i4)
                        {
                            xfer += iprot->readString(this->task_ids[_i4]);
                        }
                        xfer += iprot->readListEnd();
                    }
                    isset_task_ids = true;
                }
                else {
                    xfer += iprot->skip(ftype);
                }
                break;
            case 3:
                if (ftype == ::apache::thrift::protocol::T_I64) {
                    xfer += iprot->readI64(this->rent_end);
                    isset_rent_end = true;
                }
                else {
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

    uint32_t db_wallet_renttask::write(::apache::thrift::protocol::TProtocol* oprot) const {
        uint32_t xfer = 0;
        ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
        xfer += oprot->writeStructBegin("db_wallet_renttask");

        xfer += oprot->writeFieldBegin("rent_wallet", ::apache::thrift::protocol::T_STRING, 1);
        xfer += oprot->writeString(this->rent_wallet);
        xfer += oprot->writeFieldEnd();

        xfer += oprot->writeFieldBegin("task_ids", ::apache::thrift::protocol::T_LIST, 2);
        {
            xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->task_ids.size()));
            std::vector<std::string> ::const_iterator _iter5;
            for (_iter5 = this->task_ids.begin(); _iter5 != this->task_ids.end(); ++_iter5)
            {
                xfer += oprot->writeString((*_iter5));
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

    void swap(db_wallet_renttask& a, db_wallet_renttask& b) {
        using ::std::swap;
        swap(a.rent_wallet, b.rent_wallet);
        swap(a.task_ids, b.task_ids);
        swap(a.rent_end, b.rent_end);
    }

    db_wallet_renttask::db_wallet_renttask(const db_wallet_renttask& other6) {
        rent_wallet = other6.rent_wallet;
        task_ids = other6.task_ids;
        rent_end = other6.rent_end;
    }
    db_wallet_renttask& db_wallet_renttask::operator=(const db_wallet_renttask& other7) {
        rent_wallet = other7.rent_wallet;
        task_ids = other7.task_ids;
        rent_end = other7.rent_end;
        return *this;
    }
    void db_wallet_renttask::printTo(std::ostream& out) const {
        using ::apache::thrift::to_string;
        out << "db_wallet_renttask(";
        out << "rent_wallet=" << to_string(rent_wallet);
        out << ", " << "task_ids=" << to_string(task_ids);
        out << ", " << "rent_end=" << to_string(rent_end);
        out << ")";
    }

} // namespace