#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>
#include "Game.h"
#include "CLIListener.h"
#include "CLIPlayer.h"
#include "AIPlayer.h"
#include "ScoreKeeper.h"

// TODO:2010-06-29:zack: Make this take in some command line arguments.
// Like the number of players and AI players and whatnot.
int main(int argc, char** argv)
{
    srand(time(NULL));

    // TODO:2010-06-30:zack: Reenable this line when ScoreKeeper is updated.
    //ScoreKeeper sk;

    for (int games = 0; games < 1; games++)
    {
        std::vector<Player*> players(4);
        players[0] = new CLIPlayer("zack");
        players[1] = new CLIPlayer("erin");
        for (int i = 2; i < players.size(); i++)
        {
            std::stringstream ss;
            ss << "AIPlayer" << i;
            std::string name = ss.str();
            players[i] = new AIPlayer(name);
        }

        random_shuffle(players.begin(), players.end());

        Game game(players);

        CLIListener listener(&game);
	//game.addListener(&sk);

        game.run();
    }

    //sk.print();

    return 0;
}
