#include "deck.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include "card.h"

deck::deck()
{
    cards.reserve(36);
    for (int i = 6; i <= card::ACE; i++)
    {
        cards.push_back(card(i, card::hearts));
        cards.push_back(card(i, card::diamonds));
        cards.push_back(card(i, card::spades));
        cards.push_back(card(i, card::clubs));
    }
}

deck::~deck()
{}

card deck::deal()
{
    assert(!cards.empty());

    card ret = cards.back();
    cards.pop_back();
    return ret;
}

std::vector<card> deck::deal(int numCards)
{
    assert(numCards > 0);
    assert((int) cards.size() >= numCards);

    std::vector<card> hand(numCards);
    for (size_t i = 0; i < numCards; i++)
    {
        hand.push_back(cards.back());
        cards.pop_back();
    }

    return hand;
}

void deck::shuffle() 
{
    random_shuffle(cards.begin(), cards.end());
}

int deck::getNumCards() const
{
    return cards.size();
}

bool deck::empty() const
{
    return cards.empty();
}

void deck::print() const
{
    for (size_t i = 0; i < cards.size(); i++)
    {
        if (i != 0 && i % 9 == 0)
            std::cout << '\n';
        std::cout << cards[i] << ' ';
    }
    std::cout << '\n';
}
