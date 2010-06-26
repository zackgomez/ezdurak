#pragma once
#include <vector>
#include "Card.h"

class GameListener;
class GamePlayer;

/** Queryable interface that GameListeners are exposed to. */
class GameAgent
{
public:
    /** All intended base classes must have virtual destructors. */
    virtual ~GameAgent() {}

    virtual void addListener(GameListener *listener) = 0;
    virtual void removeListener(GameListener *listener) = 0;

    virtual Card::cardsuit getTrump() const = 0;

    virtual const GamePlayer * getAttacker() const = 0;
    virtual const GamePlayer * getDefender() const = 0;
    virtual const std::vector<GamePlayer*>& getPlayers() const = 0;

    virtual const std::vector<Card>& getPlayedCards() const = 0;
};
