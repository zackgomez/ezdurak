#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include "Game.h"
#include "CLIListener.h"
#include "CLIPlayer.h"
#include "AIPlayer.h"

// TODO:2010-06-29:zack: Make this take in some command line arguments.
// Like the number of players and AI players and whatnot.
int main()
{
    srand(time(NULL));

    std::vector<Player*> players(4);
    //players[0] = new CLIPlayer("HumanPlayer");
    for (int i = 0; i < players.size(); i++)
    {
        std::stringstream ss;
        ss << "AIPlayer" << i;
        std::string name = ss.str();
        players[i] = new AIPlayer(name);
    }

    Game game(players);

    CLIListener listener;
    game.addListener(&listener);

    game.run();

    return 0;
}
