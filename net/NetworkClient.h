#pragma once
#include "kissnet.h"
#include <string>

/** 
 * This is the client part of the autodiscovery framework.  In conjunction with
 * NetworkHost it will autodiscover and connection machines.
 */
class NetworkClient
{
public:
    /** 
     * Creates a NetworkClient object that will listen on port bport for 
     * UDP packets to autoconfigure a connection with.
     * 
     * @param bport The autodiscovery broadcast port.
     */
    NetworkClient(const std::string &bport);

    /// Destructor
    ~NetworkClient();

    /** 
     * Autodiscovers a connection by listening on bport.
     * Expects a packet that contains the port, then tries to connect to the
     * sender's address at the contained port.
     * 
     * @return A socket a machine with a NetworkHost
     */
    kissnet::tcp_socket_ptr getConnection();

private:
    std::string bport_;
    int lsock_;
};

