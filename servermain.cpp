#include <iostream>
#include "core/Game.h"
#include "net/NetworkListener.h"
#include "net/NetworkPlayer.h"
#include "net/NetworkHost.h"
#include "ai/AIPlayer.h"
#include "cli/CLIListener.h"

int main(int argc, char **argv)
{
    const int numplayers = 4;
    int numcon = 1;
    const std::string broadcastp = "12345";
    const std::string gamep = "54321";

    if (argc == 2)
        numcon = atoi(argv[1]);

    srand(time(NULL));

    NetworkHost nh(broadcastp, gamep);
    Game game;

    while (game.getNumPlayers() < numcon)
    {
        boost::shared_ptr<NetworkPlayer> netp(new NetworkPlayer());
        std::cout << "Waiting for connection " << game.getNumPlayers() + 1
            << " / " << numcon << '\n';
        kissnet::tcp_socket_ptr sock = nh.getConnection("_EZDurak");
        if (netp->getConnection(sock))
            game.addPlayer(netp);
        else
            std::cout << "Error getting connection.. trying again\n";
    }

    int i = 0;
    while (game.getNumPlayers() < numplayers)
    {
        std::string name("AIPlayer");
        name.push_back('1' + i++);
        PlayerPtr p(new AIPlayer(name));
        game.addPlayer(p);
    }


    CLIListener l;
    game.addListener(&l);

    game.run();
}
