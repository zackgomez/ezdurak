#include "NetworkClient.h"

int main()
{
    NetworkClient nc("12345");

    kissnet::tcp_socket_ptr sock = nc.getConnection();

    sock->send("Testing testing 1 2 3");
    sock->close();
}

