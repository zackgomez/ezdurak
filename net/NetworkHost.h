#pragma once
#include <string>
#include "kissnet.h"

class NetworkHost
{
public:
    NetworkHost(const std::string &bport, const std::string &lport);
    ~NetworkHost();

    kissnet::tcp_socket_ptr getConnection(const std::string &bmsg);

private:
    std::string bport_, lport_;
    int bsock_, lsock_;
    bool connected_;
};

