#include "NetworkHost.h"
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <poll.h>

using namespace kissnet;

const int NetworkHost::BROADCAST_DELAY = 500;

NetworkHost::NetworkHost(const std::string &bport, const std::string &lport) :
    bport_(bport),
    lport_(lport),
    connected_(false)
{
    struct addrinfo hints, *addr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int yes = 1;
    int status;
    // set up the broadcast (UDP) socket
    if ((bsock_ = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket (udp)");
        return;
    }
    setsockopt(bsock_, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));

    // set up the listen (TCP) socket
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, lport_.c_str(), &hints, &addr)))
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return;
    }
    lsock_ = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (setsockopt(lsock_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
    {
        perror("setsockopt (SO_REUSEADDR)");
        freeaddrinfo(addr);
        return;
    }
    // Nonblocking listen socket
    fcntl(lsock_, F_SETFL, O_NONBLOCK);
    if (bind(lsock_, addr->ai_addr, addr->ai_addrlen))
    {
        fprintf(stderr, "Unable to bind: %s\n", strerror(errno));
        freeaddrinfo(addr);
        return;
    }
    freeaddrinfo(addr);

    connected_ = true;
}

NetworkHost::~NetworkHost()
{
    if (connected_)
    {
        close(bsock_);
        close(lsock_);
    }
}

tcp_socket_ptr NetworkHost::getConnection(const std::string &bmsg)
{
    // Set up broadcast address
    struct sockaddr_in outaddr;
    memset(&outaddr, 0, sizeof(outaddr));
    outaddr.sin_family = AF_INET;
    outaddr.sin_port = htons(atoi(bport_.c_str()));
    outaddr.sin_addr.s_addr = INADDR_BROADCAST;

    // Poll struct
    struct pollfd pollee[1];
    pollee[0].fd = lsock_;
    pollee[0].events = POLLIN;

    // Set up broadcast message
    std::string msg = lport_;
    msg.push_back(' ');
    msg.append(bmsg);

    // bind and listen on the listen port
    listen(lsock_, 5);

    // Loop until a connection is found
    while (connected_)
    {
        // Broadcast a packet
        printf("DEBUG - NetworkHost: broadcasting...\n");
        if (sendto(bsock_, msg.data(), msg.size(), 0,
                (struct sockaddr *)&outaddr, sizeof(outaddr)) == -1)
        {
            perror("broadcasting: sendto");
            break;
        }

        // Wait for a connection, sleep for BROADCAST_DELAY ms
        int rv = poll(pollee, 1, BROADCAST_DELAY);
        if (rv < 0)
        {
            perror("poll");
            break;
        }
        else if (rv > 0)
        {
            // Some socket activity...
            printf("DEBUG - NetworkHost: socket activity\n");
            struct sockaddr_storage inaddr;
            socklen_t inaddrsize = sizeof(inaddr);
            int clisock = accept(lsock_, (struct sockaddr*) &inaddr, &inaddrsize);
            if (clisock)
            {
                char buf[INET_ADDRSTRLEN];
                printf("DEBUG - NetworkHost: got connection from %s\n", inet_ntop(AF_INET,
                            &(((struct sockaddr_in *)&inaddr)->sin_addr), buf, INET_ADDRSTRLEN));
                return tcp_socket::create(clisock);
            }
            else
            {
                perror("accept");
            }
        }
        // Repeat
    }

    return tcp_socket_ptr();
}