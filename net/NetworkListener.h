#pragma once
#include "core/GameListener.h"
#include "kissnet.h"

class NetworkListener :
    public GameListener
{
public:
    NetworkListener();
    virtual ~NetworkListener();

    bool getConnection(const std::string &port);

    // Methods inherited from GameListener
    void gameStart(GameAgent *);
    void gameOver(ConstPlayerPtr);
    void newRound(ConstPlayerPtr, ConstPlayerPtr);
    void endRound(bool);
    void attackerPassed(ConstPlayerPtr);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(ConstPlayerPtr player);
    void givenCards(ConstPlayerPtr player, int numCards);
    void givenCards(ConstPlayerPtr player, const std::vector<Card>& cards);

protected:
    bool connected_;
    GameAgent *agent_;
    kissnet::tcp_socket_ptr clisock_;
    std::vector<PlayerPtr> players_;
};

