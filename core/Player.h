#pragma once
#include <vector>
#include <string>
#include <set>
#include <boost/shared_ptr.hpp>
#include "Card.h"

class GameAgent;

class Player
{
public:
    Player() {}
    virtual ~Player() {}

    /**
     * Called at the start of the game.
     */
    virtual void gameStarting(GameAgent *agent) = 0;

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump) = 0;
    virtual Card attack(std::set<int> playableRanks) = 0;
    virtual Card pileOn(std::set<int> playableRanks) = 0;

    virtual void addCards(const std::vector<Card>& cards) = 0;
    virtual int getNumCards() const = 0;
    virtual std::string getName() const = 0;
};

typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::shared_ptr<const Player> ConstPlayerPtr;
