#ifndef DBC_JSON_UTIL_H
#define DBC_JSON_UTIL_H

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

// you will get reference  of a string
#define STRING_REF(VAR)  rapidjson::StringRef(VAR.c_str(), VAR.length())

// you will get A copy of a string
#define STRING_DUP(VAR)  rapidjson::Value().SetString(VAR.c_str(),VAR.length(),allocator)

#define SHORT_JSON_STRING_SIZE      256

#define JSON_PARSE_OBJECT_TO_STRING(_doc, _name, _out_val) \
    do {                                                   \
        if (!_doc.HasMember(_name))                        \
            break;                                         \
        if (!_doc[_name].IsObject())                       \
            break;                                         \
        const rapidjson::Value& obj = doc[_name];          \
        rapidjson::StringBuffer buffer;                    \
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer); \
        obj.Accept(writer);                                \
        _out_val = buffer.GetString();                     \
    } while(0);

#define JSON_PARSE_STRING(d, NAME, VAL)\
    do {\
        if( !d.HasMember(NAME)){\
            break;\
        }\
        if (! d[NAME].IsString()){\
            break;\
        }\
        VAL=d[NAME].GetString();\
    } while (0);

#define JSON_PARSE_INT(d, NAME, VAL)\
    do {\
        if( !d.HasMember(NAME)){\
            break;\
        }\
        if (! d[NAME].IsInt()){\
            break;\
        }\
        VAL=d[NAME].GetInt();\
    } while (0);

#define JSON_PARSE_UINT(d, NAME, VAL)\
    do {\
        if( !d.HasMember(NAME)){\
            break;\
        }\
        if (! d[NAME].IsUint()){\
            break;\
        }\
        VAL=d[NAME].GetUint();\
    } while (0);

#define JSON_PARSE_INT64(d, NAME, VAL)\
    do {\
        if(!d.HasMember(NAME)){\
            break;\
        }\
        if (! d[NAME].IsInt64()){\
            break;\
        }\
        VAL=d[NAME].GetInt64();\
    } while (0);

#define JSON_PARSE_UINT64(d, NAME, VAL)\
    do {\
        if( !d.HasMember(NAME)){\
            break;\
        }\
        if (! d[NAME].IsUint64()){\
            break;\
        }\
        VAL=d[NAME].GetUint64();\
    } while (0);

#define JSON_PARSE_DOUBLE(d, NAME, VAL)\
    do {\
        if( !d.HasMember(NAME)){\
            break;\
        }\
        if (! d[NAME].IsDouble()){\
            break;\
        }\
        VAL=d[NAME].GetDouble();\
    } while (0);

#endif //DBC_JSON_UTIL_H
