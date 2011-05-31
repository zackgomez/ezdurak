#pragma once
#include "CardHolder.h"
#include <vector>

class GUIPlayer;

/** 
 * A CardHolder implementation with extra methods needed for display a human
 * players cards.  These methods include getting all of the cards the CardHolder
 * currently contains.  Interfaces with the GUIPlayer class and crosschecks
 * requests with the hand that GUIPlayer holds.
 */
class HumanCardHolder :
    public CardHolder
{
public:
    HumanCardHolder(const GUIPlayer *player, Card trumpCard);
    virtual ~HumanCardHolder();

    std::vector<Card>& getCards() { return cards_; }
    const std::vector<Card>& getCards() const { return cards_; }
    unsigned getNumCards() const { return cards_.size(); }

    // Functions inherited from CardHolder interface
    virtual void addCard(Card c);
    virtual void addCards(const std::vector<Card>& cs);
    virtual void removeCard(Card c);
    virtual void clear();
    virtual bool contains(const Card &c) const;

private:
    const GUIPlayer *player_;
    std::vector<Card> cards_;
    Card trumpCard_;
};

