#include "NetworkHost.h"
#include <iostream>

int main()
{
    NetworkHost nh("12345","54321");
    kissnet::tcp_socket_ptr sock = nh.getConnection("testmsg");

    char buf[1000];
    int n = sock->recv(buf, 1000);

    std::cout << "Got message over autonegotiated connection: ";
    std::cout.write(buf, n);
    std::cout << '\n';

    sock->close();
}
