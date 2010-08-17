#pragma once
#include "CardHolder.h"
#include <vector>

class GUIPlayer;

class HumanCardHolder :
    public CardHolder
{
public:
    HumanCardHolder(const GUIPlayer *player, Card trumpCard);
    virtual ~HumanCardHolder();

    std::vector<Card>& getCards() { return cards_; }
    const std::vector<Card>& getCards() const { return cards_; }
    int getNumCards() const { return cards_.size(); }

    // Functions inherited from CardHolder interface
    virtual void addCard(Card c);
    virtual void addCards(const std::vector<Card>& cs);
    virtual void removeCard(Card c);
    virtual void clear();

private:
    const GUIPlayer *player_;
    std::vector<Card> cards_;
    Card trumpCard_;
};

