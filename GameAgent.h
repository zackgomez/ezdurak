#pragma once
#include <vector>
#include "Card.h"

class GameListener;
class Player;

/** Queryable interface that GameListeners are exposed to. */
class GameAgent
{
public:
    /** All intended base classes must have virtual destructors. */
    virtual ~GameAgent() {}

    virtual void addListener(GameListener *listener) = 0;
    virtual void removeListener(GameListener *listener) = 0;

    virtual Card getTrumpCard() const = 0;

    virtual const Player * getAttacker() const = 0;
    virtual const Player * getDefender() const = 0;
    virtual const std::vector<Player*>& getPlayers() const = 0;

    virtual const std::vector<Card>& getPlayedCards() const = 0;
};
