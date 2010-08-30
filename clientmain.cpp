#include <iostream>
#include "net/NetworkGame.h"
#include "cli/CLIListener.h"
#include "cli/CLIPlayer.h"
#include "ai/AIPlayer.h"

int main(int argc, char **argv)
{

    PlayerPtr localPlayer(new AIPlayer("networkai"));
    //PlayerPtr localPlayer(new CLIPlayer("networkme"));
    NetworkGame game(localPlayer);
    CLIListener listener;
    game.addListener(&listener);

    if(game.connectTo("localhost", "54321"))
        game.run();
    else
        std::cout << "Unable to connect, quiting\n";
}
