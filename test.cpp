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
    card c(card::ACE, card::hearts);
    card c2(10, card::hearts);
    card c3(6, card::clubs);
    card c4(8, card::diamonds);

    std::cout << "Ace of hearts:\n";
    std::cout << c << '\n';;

    std::cout << "Does " << c << " beat " << c2 << "?\n";
    std::cout << (c.beats(c2, card::none) ? yes : no) << '\n';

    std::cout << "Does " << c << " beat " << c3 << "?\n";
    std::cout << (c.beats(c3, card::none) ? yes : no) << '\n';

    std::cout << "Does " << c4 << " beat " << c << " (diamonds is trump)?\n";
    std::cout << (c4.beats(c, card::diamonds) ? yes : no) << '\n';


    // Deck Tests
    deck d;
    d.print();

    std::cout << "\nShuffling...\n";
    d.shuffle();
    d.print();

    return 0;
}
