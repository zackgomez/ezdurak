#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include "card.h"
#include "deck.h"

int main(int argc, char **argv)
{
    std::string yes = "yes";
    std::string no = "no";

    srand(time(NULL));

    // Card tests
    Card c(Card::ACE, Card::hearts);
    Card c2(10, Card::hearts);
    Card c3(6, Card::clubs);
    Card c4(8, Card::diamonds);

    std::cout << "Ace of hearts:\n";
    std::cout << c << '\n';;

    std::cout << "Does " << c << " beat " << c2 << "?\n";
    std::cout << (c.beats(c2, Card::none) ? yes : no) << '\n';

    std::cout << "Does " << c << " beat " << c3 << "?\n";
    std::cout << (c.beats(c3, Card::none) ? yes : no) << '\n';

    std::cout << "Does " << c4 << " beat " << c << " (diamonds is trump)?\n";
    std::cout << (c4.beats(c, Card::diamonds) ? yes : no) << '\n';


    // Deck Tests
    Deck d;
    d.print();

    std::cout << "\nShuffling...\n";
    d.shuffle();
    d.print();

    return 0;
}
