#pragma once
#include "Player.h"
#include "GameListener.h"

class AIPlayer :
    public Player, public GameListener
{
public:
    AIPlayer(GameAgent* agent, const std::string& name,
             const std::vector<Card>& hand);
    virtual ~AIPlayer();

    // Methods Inherited from Player interface
    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card pileOn(std::set<int> playableRanks);
    virtual void addCards(const std::vector<Card>& cards);

    // Methods Inherited from GameListener interface
    virtual void gameStart();
    virtual void gameOver(const Player*);
    virtual void attackerChanged(const Player*);
    virtual void defenderChanged(const Player*);
    virtual void defenderLost();
    virtual void defenderWon();
    virtual void attackingCard(const Card &c);
    virtual void defendingCard(const Card &c);
    virtual void playedOut(const Player *player);
    virtual void givenCards(const Player *player, int numCards);

private:
    // Helper functions
    std::vector<Card> playableCards(std::set<int> playableRanks);
    std::vector<Card> defendableCards(const Card& card, Card::cardsuit trump);
    void removeCard(const Card& card);
};
