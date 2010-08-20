#pragma once
#include "CardHolder.h"
#include <vector>
#include <cassert>
#include <algorithm>

class CardHolderImpl :
    public CardHolder
{
public:
    CardHolderImpl() { /* Empty */ }
    virtual ~CardHolderImpl() { /* Empty */ };

    std::vector<Card>& getCards() { return cards_; }
    const std::vector<Card>& getCards() const { return cards_; }
    int getNumCards() const { return cards_.size(); }

    // Functions inherited from CardHolder interface
    virtual void addCard(Card c) { cards_.push_back(c); }
    virtual void addCards(const std::vector<Card>& cs)
    {
        cards_.insert(cards_.begin(), cs.begin(), cs.end());
    }
    virtual void removeCard(Card c)
    {
        // find the card
        std::vector<Card>::iterator it = std::find(cards_.begin(), cards_.end(),
                                                   c);
        // If its not there, then look for a Card() to remove
        if (it == cards_.end())
        {
            for (it = cards_.begin(); it != cards_.end(); it++)
                if (*it == Card())
                    break;
        }
        // Make sure we found a card
        assert( it != cards_.end() );
        cards_.erase(it);
    }
    virtual void clear()
    {
        cards_.clear();
    }

    virtual bool contains(const Card &c) const
    {
        return std::find(cards_.begin(), cards_.end(), c) != cards_.end();
    }

private:
    std::vector<Card> cards_;
};

