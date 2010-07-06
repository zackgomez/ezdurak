#include <pthread.h>
#include <vector>
#include <ctime>
#include <sstream>
#include "GUIListener.h"
#include "GUIImpl.h"
#include "AIPlayer.h"
#include "Game.h"
#include "CLIListener.h"

using namespace std;

void * gui_main(void *guiobj);

int main(int argc, char **argv)
{
    srand(time(NULL));

    std::vector<Player*> players(4);
    for (int i = 0; i < players.size(); i++)
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

    game.run();

    pthread_join(gui_thread, NULL);
}

void* gui_main(void *guiobj)
{
    GUIImpl *gui = (GUIImpl*) guiobj;
    gui->run();

    pthread_exit(NULL);
}
