#pragma once
#include "CardHolder.h"

class PileCardHolder :
    public CardHolder
{
public:
    PileCardHolder() : size_(0)
    { /* Empty */ }

    PileCardHolder(int n) : size_(n)
    { /* Empty */ }

    virtual void addCard(Card c)
    {
        ++size_;
    }

    virtual void addCards(const std::vector<Card>& cs)
    {
        size_ += cs.size();
    }

    virtual void removeCard(Card c)
    {
        --size_;
        assert(size_ >= 0);
    }

    virtual void clear()
    {
        assert(false && "This method should not be called.");
    }

    virtual bool contains(const Card &c) const
    {
        assert(false && "This method should not be called.");
    }

    int getNumCards() const
    {
        return size_;
    }

private:
    int size_;

};

