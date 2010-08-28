#pragma once
#include "core/PlayerImpl.h"

/**
 * This class is used by NetworkAgent to fill in the players_
 * and give to people who request information through the GameAgent interface.
 * It only holds data and the actual methods that do work assert false.
 */
class ProxyPlayer :
    public Player
{
public:
    ProxyPlayer(const std::string &name) :
        name_(name),
        numCards_(6)
    { /* Empty */ }

    ~ProxyPlayer()
    { /* Empty */ }

    // Functions used by a proxy agent 
    void removeCards(int numCards)
    {
        numCards_ -= numCards;
    }

    // Functions implemented from that PlayerImpl/Player interface
    virtual void gameStarting(GameAgent *agent)
    { /* Empty */ }

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump)
    {
        assert(false && "This method not implemented");
        return Card();
    }

    virtual Card attack(std::set<int> playableRanks)
    {
        assert(false && "This method not implemented");
        return Card();
    }

    virtual Card pileOn(std::set<int> playableRanks)
    {
        assert(false && "This method not implemented");
        return Card();
    }

    virtual void addCards(const std::vector<Card> &cards)
    {
        numCards_ += cards.size();
    }

    virtual int getNumCards() const
    {
        return numCards_;
    }

    virtual std::string getName() const
    {
        return name_;
    }

protected:
    std::string name_;
    int numCards_;
};

