#include "NetworkClient.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <cstdio>
#include <cstring>

NetworkClient::NetworkClient(const std::string &bport) :
    bport_(bport)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int rv;
    if ((rv = getaddrinfo(NULL, bport_.c_str(), &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    if ((lsock_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        return;
    }

    if (bind(lsock_, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(lsock_);
        perror("bind:");
    }
    freeaddrinfo(res);
}

NetworkClient::~NetworkClient()
{
    close(lsock_);
}

kissnet::tcp_socket_ptr NetworkClient::getConnection()
{
    static const int MSG_SIZE = 1000;
    char buf[MSG_SIZE];
    struct sockaddr_storage fromsaddr;
    struct in_addr fromaddr;
    socklen_t addrlen = sizeof(fromsaddr);
    for (;;)
    {
        socklen_t len = addrlen;
        int n = recvfrom(lsock_, buf, MSG_SIZE, 0, (struct sockaddr *) &fromsaddr, &len);
        fromaddr = ((struct sockaddr_in *) &fromsaddr)->sin_addr;

        // Get string representation of addr
        char addrbuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &fromaddr, addrbuf, INET_ADDRSTRLEN);

        printf("DEBUG _ NetworkClient: got message from: %s\n", addrbuf);
        printf("DEBUG - NetworkClient: got data from udp sock: %.*s\n", n, buf);
    }

    return kissnet::tcp_socket_ptr();
}
