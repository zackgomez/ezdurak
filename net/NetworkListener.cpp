#include "NetworkListener.h"
#include <iostream>
#include "NetworkProtocol.h"

using namespace kissnet;

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

        // Check for handshake message
        char header[3];
        clisock_->recv(header, 3);
        if (header[2] == MSG_READY)
        {
            connected_ = true;
            std::cout << "Got connection and ready message!\n";
        }
    }
    catch (socket_exception &e)
    {
        std::cerr << "Unable to getConnection: " << e.what() << '\n';
        connected_ = false;
    }

    return connected_;
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

void NetworkListener::endRound(bool successfulDefend)
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
