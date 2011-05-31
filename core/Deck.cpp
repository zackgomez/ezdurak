#include "Deck.h"
#include <cassert>
#include <algorithm>
#include <iostream>

Deck::Deck()
{
    cards_.reserve(36);
    for (int i = 6; i <= Card::ACE; i++)
    {
        cards_.push_back(Card(i, Card::hearts));
        cards_.push_back(Card(i, Card::diamonds));
        cards_.push_back(Card(i, Card::spades));
        cards_.push_back(Card(i, Card::clubs));
    }
}

Deck::~Deck()
{}

Card Deck::deal()
{
    assert(!cards_.empty());

    Card ret = cards_.back();
    cards_.pop_back();
    return ret;
}

std::vector<Card> Deck::deal(unsigned numCards)
{
    assert(numCards > 0);
    assert(cards_.size() >= numCards);

    std::vector<Card> hand(numCards);
    for (unsigned i = 0; i < numCards; i++)
    {
        hand[i] = cards_.back();
        cards_.pop_back();
    }

    return hand;
}

Card Deck::peekLast() const
{
    assert(!cards_.empty());

    return cards_.front();
}

void Deck::shuffle() 
{
    random_shuffle(cards_.begin(), cards_.end());
}

unsigned Deck::getNumCards() const
{
    return cards_.size();
}

bool Deck::empty() const
{
    return cards_.empty();
}
