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

    for (int games = 0; games < 1; games++)
    {
        std::vector<PlayerPtr> players(4);
        //players[0] = PlayerPtr(new CLIPlayer("Zack"));
        for (int i = 0; i < players.size(); i++)
        {
            std::stringstream ss;
            ss << "AIPlayer" << i;
            std::string name = ss.str();
            players[i] = PlayerPtr(new AIPlayer(name));
        }

        random_shuffle(players.begin(), players.end());

        Game game(players);

        CLIListener listener;
	game.addListener(&sk);
        game.addListener(&listener);

        game.run();
    }

    sk.print();

    return 0;
}
