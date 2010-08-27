#pragma once
#include "core/GameAgent.h"
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
    public GameAgent
{
public:
    NetworkGame();
    ~NetworkGame();

    bool connectTo(const std::string &host, const std::string &port);

    // Functions inherited from Game interface
    virtual void run();

    // Functions inherited from GameAgent interface
    void addListener(GameListener* listener);
    void removeListener(GameListener* listener);
    Card getTrumpCard() const;
    int getTricksLeft() const;
    int getDeckSize() const;
    int getDiscardSize() const;
    ConstPlayerPtr getAttacker() const;
    ConstPlayerPtr getDefender() const;
    const std::vector<PlayerPtr> getPlayers() const;
    const std::vector<Card>& getPlayedCards() const;

private:
    kissnet::tcp_socket_ptr sock_;
    bool connected_;

    Thread read_thread_;
    SynchronizedQueue<Message> queue_;

    std::vector<PlayerPtr> players_;
    PlayerPtr attacker_;
    PlayerPtr defender_;

    int deckSize_;
    int discardSize_;
    int tricksLeft_;
    Card trumpCard_;
    std::vector<Card> playedCards_;

    std::set<GameListener*>::iterator lit_;
    std::set<GameListener*> listeners_;
};

