#ifndef DBC_HTTPCHAINCLIENT_H
#define DBC_HTTPCHAINCLIENT_H

#include "util/utils.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "util/httplib.h"

class HttpChainClient : public Singleton<HttpChainClient> {
public:
    HttpChainClient();

    virtual ~HttpChainClient();

    std::string request_machine_status();

    int64_t request_rent_end(const std::string &wallet);

    int64_t request_cur_block();

private:
    httplib::SSLClient* m_httpclient = nullptr;
};


#endif