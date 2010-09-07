#include "NetworkClient.h"
#ifndef _MSC_VER
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#define close closesocket
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>

int getLSock(const std::string &port);

void* listen_thread(void *arg);
struct listen_thread_arg {
    const std::string *port;
    std::set<NetworkClient::Connection> *conns;
    Mutex *connsMutex;
    bool *running;
};

NetworkClient::NetworkClient(const std::string &bport) :
    bport_(bport),
    thread_running_(false)
{
}

NetworkClient::~NetworkClient()
{
    if (thread_running_)
        ignore();
}

void NetworkClient::listen()
{
    conns_.clear();
    if (thread_running_)
        return;

    thread_running_ = true;

    listen_thread_arg *arg = new listen_thread_arg();
    arg->port = &bport_;
    arg->conns = &conns_;
    arg->connsMutex = &connsMutex_;
    arg->running = &thread_running_;
    lthread_.run(listen_thread, arg);
}

void NetworkClient::ignore()
{
    thread_running_ = false;
    lthread_.join();
    printf("DEBUG - NetworkClient: Joined lthread\n");
}

std::set<NetworkClient::Connection> NetworkClient::getConnectionList()
{
    Lock l(connsMutex_);
    std::set<Connection> ret(conns_);
    conns_.clear();
    return ret;
}

int getLSock(const std::string &port)
{
    int lsock;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int rv;
    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 0;
    }

    if ((lsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        return 0;
    }

    if (bind(lsock, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(lsock);
        perror("bind");
        return 0;
    }
    freeaddrinfo(res);

    return lsock;
}

void* listen_thread(void *arg)
{
    listen_thread_arg *ltarg = (listen_thread_arg *) arg;
    const std::string *port = ltarg->port;
    std::set<NetworkClient::Connection> *conns = ltarg->conns;
    Mutex *connsMutex = ltarg->connsMutex;
    bool *running = ltarg->running;
    delete ltarg;

    int lsock = getLSock(*port);

    // Set up select
    fd_set master, rdset;
    FD_ZERO(&master);
    FD_SET(lsock, &master);

    static const int DELAY_TIME = 500;
    static const int MSG_SIZE = 1000;
    char buf[MSG_SIZE];
    struct sockaddr_storage fromsaddr;
    struct in_addr fromaddr;
    socklen_t addrlen = sizeof(fromsaddr);
    while (*running)
    {
        rdset = master;
        socklen_t len = addrlen;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = DELAY_TIME * 1000;
        select(lsock+1, &rdset, NULL, NULL, &tv);
        if (!FD_ISSET(lsock, &rdset))
            continue;
        // There is data in the socket, lets get it
        int n = recvfrom(lsock, buf, MSG_SIZE, 0, (struct sockaddr *) &fromsaddr, &len);
        fromaddr = ((struct sockaddr_in *) &fromsaddr)->sin_addr;
        if (n < 0)
        {
            printf("ERROR - NetworkClient/listen_thread: bad return from recvfrom\n");
            perror("");
            *running = false;
            continue;
        }

        // Get string representation of addr
        char addrbuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &fromaddr, addrbuf, INET_ADDRSTRLEN);

        printf("DEBUG - NetworkClient/listen_thread: got message from: %s\n", addrbuf);
        printf("DEBUG - NetworkClient/listen_thread: got data from udp sock: %.*s\n", n, buf);

        std::stringstream ss;
        ss.write(buf, n);
        std::string portstr;
        ss >> portstr;

        // Is the port a number?
        if (atoi(portstr.c_str()))
        {
            // Fill out connection
            NetworkClient::Connection con;
            con.addr = addrbuf;
            con.port = portstr;
            con.aux = ss.str();

            // Lock goes out of scope after the addition of the new connection
            Lock l(*connsMutex);
            conns->insert(con);
        }
        else
        {
            printf("DEBUG - NetworkClient/listen_thread: got malformed connection, ignoring it.\n");
        }
    }

    printf("DEBUG - NetworkClient/listen_thread: closing socket\n");
    close(lsock);

    pthread_exit(NULL);
    return NULL;
}

bool operator<(const NetworkClient::Connection& lhs, const NetworkClient::Connection& rhs)
{
    if (lhs.addr < rhs.addr)
        return true;
    return lhs.port < rhs.port;
}
