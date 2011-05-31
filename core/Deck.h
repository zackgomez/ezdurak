#pragma once
#include <vector>
#include "Card.h"

/**
 * Class representing a Durak deck of 36 cards.
 * Has methods to shuffle and deal as well as peek at the bottom card
 * which is trump.
 */
class Deck
{
public:
    // Constructors
    Deck();
    ~Deck();

    /// Deals a single card
    Card deal();
    /// Deals numCard cards
    std::vector<Card> deal(unsigned numCards);

    /// Peeks at the bottom card of the deck
    Card peekLast() const;

    /// Shuffles the deck
    void shuffle();

    /// Returns the number of cards left int he deck
    unsigned getNumCards() const;
    /// Returns true iff the deck is empty
    bool empty() const;

private:
    /// The cards in the deck
    std::vector<Card> cards_;
};
