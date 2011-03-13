#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>
#include "core/Game.h"
#include "ai/AIPlayer.h"
#include "core/ScoreKeeper.h"
#include "cli/CLIListener.h"
#include "cli/CLIPlayer.h"

// TODO:2010-06-29:zack: Make this take in some command line arguments.
// Like the number of players and AI players and whatnot.
int main(int argc, char** argv)
{
    srand(time(NULL));

    ScoreKeeper sk;

    const int numPlayers = 4;

    for (int games = 0; games < 1; games++)
    {
        Game game;

        PlayerPtr humanPlayer(new CLIPlayer("HumanPlayer"));
        game.addPlayer(humanPlayer);
        for (int i = game.getNumPlayers(); i < numPlayers; i++)
        {
            std::stringstream ss;
            ss << "AIPlayer" << i;
            std::string name = ss.str();

            PlayerPtr player(new AIPlayer(name));
            game.addPlayer(player);
        }

        CLIListener listener;
        game.addListener(&sk);
        game.addListener(&listener);

        game.run();
    }

    sk.print();

    return 0;
}
