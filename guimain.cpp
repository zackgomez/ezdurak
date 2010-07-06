#include <pthread.h>
#include <vector>
#include <ctime>
#include "GUIImpl.h"
#include "Deck.h"

void * game_main(void *guiobj);

int main(int argc, char **argv)
{
    srand(time(NULL));

    Deck deck;
    deck.shuffle();
    std::vector<Card> playedCards = deck.deal(9);

    GUIImpl gui;

    pthread_t game_thread;
    pthread_create(&game_thread, NULL, game_main, &gui);

    sleep(2);
    gui.setPlayedCards(playedCards);

    pthread_join(game_thread, NULL);
}

void* game_main(void *guiobj)
{
    GUIImpl *gui = (GUIImpl*) guiobj;
    gui->run();

    pthread_exit(NULL);
}
