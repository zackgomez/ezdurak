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
    bool beats(Card &rhs, cardsuit trump);

    // Accessors
    int getNum() const;
    cardsuit getSuit() const;

    // Operator Overloads
    bool operator==(const Card& rhs) const;
    bool operator!=(const Card& rhs) const;

    operator bool() const;


private:
    int num_;
    cardsuit suit_;
};

// Operator Overloads
std::ostream& operator<<(std::ostream& os, const Card &c);
