#pragma once
#include <vector>
#include "Card.h"

class Deck
{
public:
    Deck();
    ~Deck();

    Card deal();
    std::vector<Card> deal(int numCards);

    Card peekLast() const;

    void shuffle();

    int getNumCards() const;
    bool empty() const;

    void print() const;

private:
    std::vector<Card> cards_;
};
