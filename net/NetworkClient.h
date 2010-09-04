#pragma once
#include "kissnet.h"
#include <string>

class NetworkClient
{
public:
    NetworkClient(const std::string &bport);
    ~NetworkClient();

    kissnet::tcp_socket_ptr getConnection();

private:
    std::string bport_;
    int lsock_;
};

