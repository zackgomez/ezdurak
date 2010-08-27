#include "NetworkGame.h"
#include <iostream>
#include "core/Game.h"
#include "core/GameListener.h"

NetworkGame::NetworkGame() :
    connected_(false)
{
    /* Empty */
}

NetworkGame::~NetworkGame()
{
    if (connected_)
        sock_->close();
}

bool NetworkGame::connectTo(const std::string &host, const std::string &port)
{
    try
    {
        sock_->connect(host, port);
        // TODO handshake
        // TODO start socket reader thread
        connected_ = true;
    }
    catch (kissnet::socket_exception &e)
    {
        std::cerr << "Unable to connect to " << host << ":" << port << e.what()
            << '\n';
        return false;
    }

    return true;
}

void NetworkGame::addListener(GameListener *listener)
{
}

void NetworkGame::removeListener(GameListener *listener)
{
}

Card NetworkGame::getTrumpCard() const
{
    return trumpCard_;
}

int NetworkGame::getTricksLeft() const
{
    return tricksLeft_;
}

int NetworkGame::getDeckSize() const
{
    return deckSize_;
}

int NetworkGame::getDiscardSize() const
{
    return discardSize_;
}

ConstPlayerPtr NetworkGame::getAttacker() const
{
    return attacker_;
}

ConstPlayerPtr NetworkGame::getDefender() const
{
    return defender_;
}

const std::vector<PlayerPtr> NetworkGame::getPlayers() const
{
    return players_;
}

const std::vector<Card>& NetworkGame::getPlayedCards() const
{
    return playedCards_;
}
