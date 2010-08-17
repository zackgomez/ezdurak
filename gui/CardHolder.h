#pragma once
#include "core/Card.h"
#include <vector>

class CardHolder
{
public:
    virtual ~CardHolder() { /* Empty */ }

    virtual void addCard(Card c) = 0;
    virtual void addCards(const std::vector<Card>& cs) = 0;
    virtual void removeCard(Card c) = 0;
    virtual void clear() = 0;
};

