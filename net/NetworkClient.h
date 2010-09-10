#pragma once
#include "kissnet.h"
#include <string>
#include <set>
#include "util/Thread.h"

/** 
 * This is the client part of the autodiscovery framework.  In conjunction with
 * NetworkHost it will autodiscover and connection machines.
 */
class NetworkClient
{
public:
    struct Connection
    {
        std::string addr;
        std::string port;
        std::string aux;

        bool operator<(const Connection& lhs) const;
    };

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
     * Starts for available connections on the broadcast port.
     */
    void listen();

    /** 
     * Stops listening for available connections on the broadcast port.
     */
    void ignore();

    /** 
     * Gets a list of available connections that have been discovered.
     * Only returns connects (re)discovered from the last call.  A connection
     * is returned as a Connection struct that contains the port, address, and
     * any auxiliary data.  The port is 0 if it was not found.
     * 
     * @return A list of connections.
     */
    std::set<Connection> getConnectionList();

private:
    std::set<Connection> conns_;
    Thread lthread_;
    Mutex  connsMutex_;
    bool thread_running_;

    std::string bport_;
};