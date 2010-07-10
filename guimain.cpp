#include <pthread.h>
#include <vector>
#include <ctime>
#include <sstream>
#include "GUIListener.h"
#include "GUIImpl.h"
#include "AIPlayer.h"
#include "Game.h"
#include "CLIListener.h"
#include "CLIPlayer.h"
#include "GUIPlayer.h"

using namespace std;

void * gui_main(void *guiobj);
void * game_main(void *gameobj);

int main(int argc, char **argv)
{
    srand(time(NULL));

    std::vector<Player*> players(4);
    players[0] = new GUIPlayer("guiplayer");
    for (int i = 1; i < players.size(); i++)
    {
        stringstream ss;
        ss << "AIPlayer" << i;
        std::string name = ss.str();
        players[i] = new AIPlayer(name);
    }

    GUIImpl gui;
    Game game(players);
    GUIListener listener(&game, &gui);
    CLIListener text(&game);

    pthread_t gui_thread;
    pthread_create(&gui_thread, NULL, gui_main, &gui);

    pthread_t game_thread;
    pthread_create(&game_thread, NULL, game_main, &game);

    pthread_join(gui_thread, NULL);

    return 0;
}

void* gui_main(void *guiobj)
{
    GUIImpl *gui = (GUIImpl*) guiobj;
    gui->run();

    pthread_exit(NULL);
}

void* game_main(void *gameobj)
{
    Game *game = (Game*) gameobj;
    game->run();

    pthread_exit(NULL);
}
