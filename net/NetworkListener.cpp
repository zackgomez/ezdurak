#include "NetworkListener.h"
#include "NetworkProtocol.h"
#include "core/GameAgent.h"
#include <iostream>
#include "NetworkProtocol.h"

using namespace kissnet;
using std::string;

NetworkListener::NetworkListener() :
    connected_(false)
{
}

NetworkListener::~NetworkListener()
{
    if (connected_)
        clisock_->close();
}

bool NetworkListener::getConnection(kissnet::tcp_socket_ptr sock)
{
    clisock_ = sock;

    connected_ = doHandshake();

    return connected_;
}

bool NetworkListener::getConnection(const std::string &port)
{
    try
    {
        tcp_socket_ptr servsock = tcp_socket::create();
        servsock->listen(port, 5);
        clisock_ = servsock->accept();

        // Check for handshake message
        connected_ = doHandshake();
    }
    catch (socket_exception &e)
    {
        std::cerr << "Unable to getConnection: " << e.what() << '\n';
        connected_ = false;
    }

    return connected_;
}

bool NetworkListener::doHandshake()
{
    bool ready = false;
    while (!ready)
    {
        char header[3];
        clisock_->recv(header, 3);
        if (header[2] == MSG_READY)
        {
            std::cout << "Got connection and ready message!\n";
            ready = true;
        }
        else
        {
            std::cout << "Got unknown message during handshake\n";
            std::cout << "Header: "; std::cout.write(header,3);
            std::cout << '\n';
        }
    }

    return true;
}

void NetworkListener::gameStart(GameAgent *agent)
{
    agent_ = agent;
    players_ = agent_->getPlayers();
    string payload = serializeCard(agent_->getTrumpCard());
    char size = players_.size();
    payload.push_back(size);
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
    string end = createMessage(MSG_END, "");
    clisock_->send(end);
}

void NetworkListener::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
    string payload = serializePlayer(attacker, players_);
    payload.append(serializePlayer(defender, players_));
    string message = createMessage(MSG_NEWROUND, payload);
    clisock_->send(message);
}

void NetworkListener::attackerPassed(ConstPlayerPtr newAttacker)
{
    string payload = serializePlayer(newAttacker, players_);
    string message = createMessage(MSG_ATTACKERPASSED, payload);
    clisock_->send(message);
}

void NetworkListener::attackingCard(const Card &c)
{
    string payload = serializeCard(c);
    string message = createMessage(MSG_ATTACKINGCARD, payload);
    clisock_->send(message);
}

void NetworkListener::defendingCard(const Card &c)
{
    string payload = serializeCard(c);
    string message = createMessage(MSG_DEFENDINGCARD, payload);
    clisock_->send(message);
}

void NetworkListener::piledOnCard(const Card &c)
{
    string payload = serializeCard(c);
    string message = createMessage(MSG_PILEDONCARD, payload);
    clisock_->send(message);
}

void NetworkListener::playedOut(ConstPlayerPtr player)
{
    string payload = serializePlayer(player, players_);
    string message = createMessage(MSG_PLAYEDOUT, payload);
    clisock_->send(message);
}

void NetworkListener::givenCards(ConstPlayerPtr player, int numCards)
{
   string payload = serializePlayer(player, players_);
   payload.push_back((char) numCards);
   string message = createMessage(MSG_GIVENCARDSN, payload);
   clisock_->send(message);
}

void NetworkListener::givenCards(ConstPlayerPtr player, const std::vector<Card>& cards)
{
    string payload = serializePlayer(player, players_);
    payload.append(serializeCards(cards));
    string message = createMessage(MSG_GIVENCARDSCS, payload);
    clisock_->send(message);
}

void NetworkListener::endRound(bool successfulDefend)
{
    string payload = serializeBool(successfulDefend);
    string message = createMessage(MSG_ENDROUND, payload);
    clisock_->send(message);
}
