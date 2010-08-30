#include <iostream>
#include "net/NetworkGame.h"
#include "cli/CLIListener.h"

int main(int argc, char **argv)
{
    NetworkGame game;
    CLIListener listener;
    game.addListener(&listener);
    if(game.connectTo("localhost", "54321"))
        game.run();
    else
        std::cout << "Unable to connect, quiting\n";
}
