#pragma once
#include <vector>
#include "card.h"

class Deck
{
public:
    Deck();
    ~Deck();

    Card deal();
    std::vector<Card> deal(int numCards);

    void shuffle();

    int getNumCards() const;
    bool empty() const;

    void print() const;

private:
    std::vector<Card> cards_;
};
