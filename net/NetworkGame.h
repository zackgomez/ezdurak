#pragma once
#include "core/Game.h"
#include <set>
#include "kissnet.h"
#include "util/Thread.h"
#include "util/SynchronizedQueue.h"
#include "util/Logger.h"

/** 
 * A Proxy Game object.  Receives information from a NetworkListener or maybe
 * a NetworkPlayer.  Forwards those messages through the GameAgent interface to
 * any attached GameListeners.  Also forwards any messages from the Player 
 * interface if constructed with a player and connected to a NetworkPlayer.
 */
class NetworkGame :
    public Game
{
public:
    /** 
     * Creates a NetworkGame object that will not have a localPlayer and will
     * only relay GameListener messages.
     */
    NetworkGame();
    /// Destructor
    ~NetworkGame();

    /** 
     * Does any handshaking needed on the already connected socket passed.
     * 
     * @param sock A socket that is already connected
     * 
     * @return true if successful, false otherwise
     */
    bool connectTo(kissnet::tcp_socket_ptr sock);
    /** 
     * Connets to the given host:port combination and then commences the 
     * handshake.
     * 
     * @param host The host to connect to IP or hostname format
     * @param port The port to connect on
     * 
     * @return  true if successful, false otherwise
     */
    bool connectTo(const std::string &host, const std::string &port);

    // Message handlers
    void endMessage(const std::string &payload);
    void gameStartingMessage(const std::string &payload);
    void gameOverMessage(const std::string &payload);
    void newRoundMessage(const std::string &payload);
    void endRoundMessage(const std::string &payload);
    void attackerPassedMessage(const std::string &payload);
    void attackingCardMessage(const std::string &payload);
    void defendingCardMessage(const std::string &payload);
    void piledOnCardMessage(const std::string &payload);
    void playedOutMessage(const std::string &payload);
    void givenCardsNMessage(const std::string &payload);
    void givenCardsCSMessage(const std::string &payload);
    void attackMessage(const std::string &payload);
    void defendMessage(const std::string &payload);
    void pileOnMessage(const std::string &payload);
    void addCardsMessage(const std::string &payload);

    // Functions overriden from Game interface
    virtual void addPlayer(PlayerPtr player);
    virtual void run();

    // Functions overriden from GameAgent interface
    virtual unsigned getDeckSize() const;
    virtual unsigned getDiscardSize() const;

    // Types
    struct Message
    {
        char type;
        std::string payload;
    };

private:

    kissnet::tcp_socket_ptr sock_;
    bool connected_;
    bool running_;

    Thread game_thread_;
    SynchronizedQueue<Message> queue_;

    PlayerPtr localPlayer_;
    int deckSize_;
    int discardSize_;

    LoggerPtr logger_;
};

