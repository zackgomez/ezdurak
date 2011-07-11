#pragma once
#include "core/GameListener.h"
#include "kissnet.h"
#include "util/Logger.h"

/** 
 * This class forwards GameListener messages over the network.
 */
class NetworkListener :
    public GameListener
{
public:
    /** 
     * Creates an unconnected NetworkListener object.
     */
    NetworkListener();
    /// Destructor
    virtual ~NetworkListener();

    /** 
     * Uses the already connected socket passed and commences the handshake.
     * 
     * @param sock A connected socket.
     * 
     * @return true if the handshake succeeds
     */
    bool getConnection(kissnet::tcp_socket_ptr sock);
    /** 
     * Listens for a connection on the passed port and then handshakes with the
     * connected client.
     * 
     * @param port The port to listen on.
     * 
     * @return  true if successful
     */
    bool getConnection(const std::string &port);

    // Methods inherited from GameListener
    void gameStart(GameAgent *);
    void gameOver(ConstPlayerPtr, ConstPlayerPtr);
    void newRound(ConstPlayerPtr, ConstPlayerPtr);
    void endRound(bool);
    void attackerPassed(ConstPlayerPtr);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void deflectedCard(const Card &c, ConstPlayerPtr newAttacker,
            ConstPlayerPtr newDefender);
    void piledOnCard(const Card &c);
    void playedOut(ConstPlayerPtr player);
    void givenCards(ConstPlayerPtr player, int numCards);
    void givenCards(ConstPlayerPtr player, const std::vector<Card>& cards);

protected:
    /** 
     * @brief Negotiates the handshake for the connection.
     * 
     * @return True if the handshake was successful, false otherwise.
     */
    virtual bool doHandshake();

    bool connected_;
    GameAgent *agent_;
    kissnet::tcp_socket_ptr clisock_;
    std::vector<PlayerPtr> players_;

    LoggerPtr logger_;
};

