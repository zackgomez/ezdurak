#pragma once
#include "core/Game.h"
#include <set>
#include "kissnet.h"
#include "util/Thread.h"
#include "util/SynchronizedQueue.h"

struct Message
{
    char type;
    std::string payload;
};

class NetworkGame :
    public Game
{
public:
    NetworkGame();
    NetworkGame(PlayerPtr localPlayer);
    ~NetworkGame();

    bool connectTo(kissnet::tcp_socket_ptr sock);
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
    virtual void run();

    // Functions overriden from GameAgent interface
    virtual int getDeckSize() const;
    virtual int getDiscardSize() const;

private:
    kissnet::tcp_socket_ptr sock_;
    bool connected_;
    bool running_;

    Thread game_thread_;
    SynchronizedQueue<Message> queue_;

    PlayerPtr localPlayer_;
    int deckSize_;
    int discardSize_;
};

