#include "card.h"

Card::Card()
    : num(NONUM), suit(none)
{}

Card::Card(int n, Card::cardsuit s)
    : num(n), suit(s)
{}

int Card::getNum() const
{
    return num;
}

Card::cardsuit Card::getSuit() const
{
    return suit;
}

bool Card::beats(Card &rhs, cardsuit trump)
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

std::ostream& operator<<(std::ostream &os, const Card &c)
{
    bool red = (c.getSuit() == Card::diamonds || c.getSuit() == Card::hearts);
    // Set output background to white
    os << "\033[47m";
    // Set the foreground output color to red
    if (red)
        os << "\033[31m";
    else
        os << "\033[30m";

    // If the number isn't '10' then print a space so that all cards take 3
    // spaces
    if (c.getNum() != 10)
        os << ' ';
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
