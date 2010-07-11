#pragma once
#include <vector>
#include <string>
#include <set>
#include "Card.h"

class GameAgent;

class Player
{
public:
    Player(const std::string& name);
    virtual ~Player();

    /**
     * Called at the start of the game.
     */
    virtual void gameStarting(GameAgent *agent) = 0;

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump) = 0;
    // TODO:2010-07-10:zack: Removed the default argument.
    virtual Card attack(std::set<int> playableRanks = std::set<int>()) = 0;
    virtual Card pileOn(std::set<int> playableRanks) = 0;

    virtual void addCards(const std::vector<Card>& cards);
    int getNumCards() const;
    std::string getName() const;

    void print() const;

protected:
    std::vector<Card> hand_;
    std::string name_;

private:
    // Private copy and assignment
    Player(const Player&);
    void operator=(const Player&);
};

