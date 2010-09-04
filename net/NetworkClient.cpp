#include "NetworkClient.h"
#include <sys/socket.h>

NetworkClient::NetworkClient(const std::string &bport) :
    bport_(bport)
{
    char yes = 1;
    if ((lsock_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        return;
    }
}

NetworkClient::~NetworkClient()
{
    close(lsock_);
}

kissnet::tcp_socket_ptr NetworkClient::getConnection()
{
    static const int MSG_SIZE = 1000;
    char buf[MSG_SIZE];
    struct sockaddr_storage fromaddr;
    socklen_t addrlen = sizeof(fromaddr);
    for (;;)
    {
        socklen_t len = addrlen;
        n = recvfrom(lsock_, buf, MSG_SIZE, 0, (struct sockaddr *) &fromaddr, &len);

        printf("DEBUG - NetworkClient: got data from udp sock\n");
    }
    return kissnet::tcp_socket_ptr();
}
