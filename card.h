#pragma once
#include <ostream>

class card
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
    card();
    card(int num, cardsuit suit);


    // Member Functions
    bool beats(card &rhs, cardsuit trump);

    // Accessors
    int getNum() const;
    cardsuit getSuit() const;


private:
    int num;
    cardsuit suit;
};

// Operator Overloads
std::ostream& operator<<(std::ostream& os, const card &c);
