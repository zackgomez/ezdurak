#include <iostream>
#include <string>
#include "net/NetworkGame.h"
#include "net/NetworkClient.h"
#include "cli/CLIListener.h"
#include "cli/CLIPlayer.h"
#include "ai/AIPlayer.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " playername\n";
        return 1;
    }
    std::string name = argv[1];

    //PlayerPtr localPlayer(new AIPlayer("networkai"));
    PlayerPtr localPlayer(new CLIPlayer(name));
    
    // Find a game to play...
    std::cout << "Looking for a game\n";
    NetworkClient gamefinder("12345");

    kissnet::tcp_socket_ptr sock = gamefinder.getConnection();

    NetworkGame game;
    game.addPlayer(localPlayer);
    CLIListener listener;
    game.addListener(&listener);

    if(game.connectTo(sock))
        game.run();
    else
        std::cout << "Unable to connect, quiting\n";
}
