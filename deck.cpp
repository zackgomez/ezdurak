#include "deck.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include "card.h"

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

std::vector<Card> Deck::deal(int numCards)
{
    assert(numCards > 0);
    assert((int) cards_.size() >= numCards);

    std::vector<Card> hand(numCards);
    for (int i = 0; i < numCards; i++)
    {
        hand.push_back(cards_.back());
        cards_.pop_back();
    }

    return hand;
}

void Deck::shuffle() 
{
    random_shuffle(cards_.begin(), cards_.end());
}

int Deck::getNumCards() const
{
    return cards_.size();
}

bool Deck::empty() const
{
    return cards_.empty();
}

void Deck::print() const
{
    for (size_t i = 0; i < cards_.size(); i++)
    {
        if (i != 0 && i % 9 == 0)
            std::cout << '\n';
        std::cout << cards_[i] << ' ';
    }
    std::cout << '\n';
}
