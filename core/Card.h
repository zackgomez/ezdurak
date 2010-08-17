#pragma once
#include <ostream>

class Card
{
public:
    // Types
    enum cardsuit { none = 0, hearts, diamonds, spades, clubs };
    static const int NONUM = -1;
    static const int JACK  = 11;
    static const int QUEEN = 12;
    static const int KING  = 13;
    static const int ACE   = 14;

    // Constructors
    Card();
    Card(int num, cardsuit suit);


    // Member Functions
    bool beats(const Card &rhs, cardsuit trump) const;

    // Accessors
    int getNum() const;
    cardsuit getSuit() const;

    // Operator Overloads
    bool operator==(const Card& rhs) const;
    bool operator!=(const Card& rhs) const;

    // Returns false if this card is the default card
    operator bool() const;


private:
    int num_;
    cardsuit suit_;
};

// Operator Overloads
std::ostream& operator<<(std::ostream& os, const Card &c);

// Card comparison functor
class CardComp
{
private:
    Card::cardsuit trump;
public:
    CardComp(Card::cardsuit trumpsuit) :
        trump(trumpsuit)
    {}

    bool operator()(const Card &a, const Card &b)
    {
        Card::cardsuit aSuit = a.getSuit();
        Card::cardsuit bSuit = b.getSuit();

        // Case 1: one trump, one not trump
        if ((aSuit == trump && bSuit != trump) || 
            (bSuit == trump && aSuit != trump))
        {
            return a.getSuit() != trump;
        }
        // Case 2: both trump or both not trump
        return a.getNum() < b.getNum();
    }
};
