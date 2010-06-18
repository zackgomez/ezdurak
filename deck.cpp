#include "deck.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include "card.h"

Deck::Deck()
{
    cards.reserve(36);
    for (int i = 6; i <= Card::ACE; i++)
    {
        cards.push_back(Card(i, Card::hearts));
        cards.push_back(Card(i, Card::diamonds));
        cards.push_back(Card(i, Card::spades));
        cards.push_back(Card(i, Card::clubs));
    }
}

Deck::~Deck()
{}

Card Deck::deal()
{
    assert(!cards.empty());

    Card ret = cards.back();
    cards.pop_back();
    return ret;
}

std::vector<Card> Deck::deal(int numCards)
{
    assert(numCards > 0);
    assert((int) cards.size() >= numCards);

    std::vector<Card> hand(numCards);
    for (size_t i = 0; i < numCards; i++)
    {
        hand.push_back(cards.back());
        cards.pop_back();
    }

    return hand;
}

void Deck::shuffle() 
{
    random_shuffle(cards.begin(), cards.end());
}

int Deck::getNumCards() const
{
    return cards.size();
}

bool Deck::empty() const
{
    return cards.empty();
}

void Deck::print() const
{
    for (size_t i = 0; i < cards.size(); i++)
    {
        if (i != 0 && i % 9 == 0)
            std::cout << '\n';
        std::cout << cards[i] << ' ';
    }
    std::cout << '\n';
}
