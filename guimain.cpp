#include <pthread.h>
#include <vector>
#include <ctime>
#include <sstream>
#include <algorithm>
#include "core/Game.h"
#include "ai/AIPlayer.h"
#include "gui/GUIListener.h"
#include "gui/GUIImpl.h"
#include "gui/GUIPlayer.h" 
using namespace std;

void * gui_main(void *guiobj);
void * game_main(void *gameobj);

int main(int argc, char **argv)
{
    srand(time(NULL));

    SynchronizedQueue<int> q;
    std::vector<Player*> players(4);
    players[0] = new GUIPlayer("guiplayer", q);
    for (int i = 1; i < players.size(); i++)
    {
        stringstream ss;
        ss << "AIPlayer" << i;
        std::string name = ss.str();
        players[i] = new AIPlayer(name);
    }
    std::random_shuffle(players.begin(), players.end());

    GUIImpl gui;
    Game game(players);
    GUIListener listener(&game, &gui);

    pthread_t gui_thread;
    pthread_create(&gui_thread, NULL, gui_main, &gui);

    pthread_t game_thread;
    pthread_create(&game_thread, NULL, game_main, &game);

    pthread_join(gui_thread, NULL);

    return 0;
}

void* gui_main(void *guiobj)
{
    srand(time(NULL));
    GUIImpl *gui = (GUIImpl*) guiobj;
    gui->run();

    pthread_exit(NULL);
	return NULL;
}

void* game_main(void *gameobj)
{
    srand(time(NULL));
    Game *game = (Game*) gameobj;
    game->run();

    pthread_exit(NULL);
	return NULL;
}
