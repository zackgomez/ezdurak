#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <vector>
#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "CLIPlayer.h"

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

    std::cout << "Dealing a couple cards...\n";
    std::cout << d.deal();
    std::cout << d.deal();
    std::cout << '\n';

    // GamePlayer Tests
    std::cout << "Give a player a hand...\n";
    std::string name = "TestPlayer";
    std::vector<Card> hand = d.deal(15);
    Player* player = new CLIPlayer(name);
    player->addCards(hand);
    player->print();
    std::cout << '\n';

    // -- Defending Tests
    std::cout << "Asking the player to defend a 6 of clubs...\n";
    Card attackingCard(6, Card::clubs);
    Card defendingCard = player->defend(attackingCard, Card::none);

    if (defendingCard != Card())
        std::cout << "They defended with " << defendingCard << '\n';
    else 
        std::cout << "They gave up\n";
    player->print();

    // -- Attack Tests
    std::cout << "Asking them to attack with a first card...\n'";
    attackingCard = player->attack();
    std::cout << "They played a " << attackingCard << '\n';

    player->print();
    delete player;


    return 0;
}
