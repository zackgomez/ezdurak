#include "GUIImpl.h"
#include <vector>
#include <ctime>
#include "Deck.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    Deck deck;
    deck.shuffle();
    std::vector<Card> playedCards = deck.deal(9);

    GUIImpl gui;
    gui.setPlayedCards(playedCards);
    gui.run();

    return 0;
}
