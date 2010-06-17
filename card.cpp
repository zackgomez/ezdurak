#include "card.h"

card::card()
    : num(NONUM), suit(none)
{}

card::card(int n, card::cardsuit s)
    : num(n), suit(s)
{}

int card::getNum() const
{
    return num;
}

card::cardsuit card::getSuit() const
{
    return suit;
}

bool card::beats(card &rhs, cardsuit trump)
{
    // We only care if we are trump and they are not, we beat them then, other
    // case falls through to the return false.
    if (suit == trump && rhs.suit != trump)
        return true;
    // If they are the same suit, it's based on the number, even if its trump
    if (suit == rhs.suit)
        return num > rhs.num;
    // If it's not the same suit or some trump thing, return false.
    return false;
}

std::ostream& operator<<(std::ostream &os, const card &c)
{
    bool red = (c.getSuit() == card::diamonds || c.getSuit() == card::hearts);
    // Set the output color to red
    if (red)
        os << "\033[31m";

    // Print number
    char numchars[4] = {'J', 'Q', 'K', 'A'};
    if (c.getNum() <= 10)
        os << c.getNum();
    else
        os << numchars[c.getNum() - 11];

    // Print suit
    char suitchars[5] = {'!', 'H', 'D', 'S', 'C'};
    os << suitchars[c.getSuit()];

    // Reset colors
    os << "\033[0m";

    return os;
}
