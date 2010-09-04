#include "NetworkClient.h"

int main()
{
    NetworkClient nc("12345");

    kissnet::tcp_socket_ptr sock = nc.getConnection();
}

