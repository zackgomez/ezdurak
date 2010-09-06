#pragma once
#include <string>
#include "kissnet.h"

/** 
 * This is the server half of the autodiscovery framework.  In conjunction with
 * NetworkClient it will autodiscover and connect two machines.
 */
class NetworkHost
{
public:
    /** 
     * Creates a NetworkHost that will autodiscover on port bport and negotiate
     * connects to port lport.
     * 
     * @param bport The autodiscovery port.
     * @param lport The connection port.
     */
    NetworkHost(const std::string &bport, const std::string &lport);
    ~NetworkHost();

    /** 
     * The delay between broadcast packets.
     */
    static const int BROADCAST_DELAY;

    /** 
     * Autodiscovers a connection with a NetworkClient and then returns the
     * connected socket.
     * 
     * @param bmsg Additional data to send with the broadcast packets.
     * 
     * @return A connected, autodiscovered socket.
     */
    kissnet::tcp_socket_ptr getConnection(const std::string &bmsg);

private:
    std::string bport_, lport_;
    int bsock_, lsock_;
    bool connected_;

    struct timeval last_bcast_;
};

