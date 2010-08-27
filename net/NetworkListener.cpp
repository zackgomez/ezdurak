#include "NetworkListener.h"
#include <iostream>

using namespace kissnet;

NetworkListener::NetworkListener() :
    connected_(false)
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

    return true;

}

void NetworkListener::gameStart()
{
}

void NetworkListener::gameOver(ConstPlayerPtr biscuit)
{
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
