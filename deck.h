#pragma once
#include <vector>
#include "card.h"

class deck
{
public:
    deck();
    ~deck();

    card deal();
    std::vector<card> deal(int numCards);

    void shuffle();

    int getNumCards() const;
    bool empty() const;

    void print() const;

private:
    std::vector<card> cards;
};
