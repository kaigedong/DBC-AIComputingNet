
#include "matrix_capacity.h"

namespace matrix
{
    namespace core
    {

        std::string matrix_capacity::THRIFT_BINARY_C_NAME = std::string("thrift_binary");
        std::string matrix_capacity::THRIFT_COMPACT_C_NAME = std::string("thrift_compact");
        std::string matrix_capacity::SNAPPY_RAW_C_NAME = std::string("snappy_raw");

        // by default: thrift_binary
        matrix_capacity::matrix_capacity() :
                m_thrift_binary(true),
                m_thrift_compact(false),
                m_snappy_raw(false)
        {

        }

        bool matrix_capacity::thrift_binary() const
        {
            return m_thrift_binary;
        }

        bool matrix_capacity::snappy_raw() const
        {
            return m_snappy_raw;
        }

        void matrix_capacity::add(std::string c)
        {
            if(c.find(THRIFT_BINARY_C_NAME) == std::string::npos)
            {
                m_thrift_binary = true;
            }

            if(c.find(THRIFT_COMPACT_C_NAME) == std::string::npos)
            {
                m_thrift_compact = true;
            }

            if(c.find(SNAPPY_RAW_C_NAME) == std::string::npos)
            {
                m_snappy_raw = true;
            }
        }

        std::string matrix_capacity::to_string() const
        {
            std::string c;

            if(m_thrift_binary)
            {
                c += THRIFT_BINARY_C_NAME;
            }

            if(m_thrift_compact)
            {
                if(!c.empty()) c +=";";

                c += THRIFT_COMPACT_C_NAME;
            }

            if(m_snappy_raw)
            {
                if(!c.empty()) c +=";";
                c += SNAPPY_RAW_C_NAME;
            }

            return c;
        }

    }
}


