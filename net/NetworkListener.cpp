#include "NetworkListener.h"
#include "NetworkProtocol.h"
#include "core/GameAgent.h"
#include <iostream>
#include <string>

using namespace kissnet;
using std::string;

NetworkListener::NetworkListener(GameAgent *agent) :
    connected_(false),
    agent_(agent)
{
}

NetworkListener::~NetworkListener()
{
    if (connected_)
        clisock_->close();
}

bool NetworkListener::getConnection(const std::string &port)
{
    try
    {
    tcp_socket_ptr servsock = tcp_socket::create();
    servsock->listen(port, 5);
    clisock_ = servsock->accept();

    // TODO check for handshake

    connected_ = true;
    }
    catch (socket_exception &e)
    {
        std::cerr << "Unable to getConnection: " << e.what() << '\n';
        return false;
    }

    std::cout << "Got connection\n";

    return true;
}

void NetworkListener::gameStart()
{
    string payload = serializeCard(agent_->getTrumpCard());
    payload.push_back(players_.size());
    for (int i = 0; i < players_.size(); i++)
    {
        payload.append(serializeString(players_[i]->getName()));
    }
    string message = createMessage(MSG_GAMESTARTING, payload);
    clisock_->send(message);
}

void NetworkListener::gameOver(ConstPlayerPtr biscuit)
{
    string serialBiscuit = serializePlayer(biscuit, players_);
    string message = createMessage(MSG_GAMEOVER, serialBiscuit);
    clisock_->send(message);
}

void NetworkListener::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
}

void NetworkListener::attackerPassed(ConstPlayerPtr newAttacker)
{
}

void NetworkListener::attackingCard(const Card &c)
{
}

void NetworkListener::defendingCard(const Card &c)
{
}

void NetworkListener::piledOnCard(const Card &c)
{
}

void NetworkListener::playedOut(ConstPlayerPtr player)
{
}

void NetworkListener::givenCards(ConstPlayerPtr player, int numCards)
{
}

void NetworkListener::givenCards(ConstPlayerPtr, const std::vector<Card>& cards)
{
}
