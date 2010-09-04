#include <iostream>
#include "core/Game.h"
#include "net/NetworkListener.h"
#include "net/NetworkPlayer.h"
#include "net/NetworkHost.h"
#include "ai/AIPlayer.h"
#include "cli/CLIListener.h"

int main()
{
    const int numcon = 1;
    const int numplayers = 4;
    const std::string broadcastp = "12345";
    const std::string gamep = "54321";

    NetworkHost nh(broadcastp, gamep);

    std::vector<PlayerPtr> players;
    while (players.size() < numcon)
    {
        boost::shared_ptr<NetworkPlayer> netp(new NetworkPlayer());
        std::cout << "Waiting for connection " << players.size() + 1
            << " / " << numcon << '\n';
        kissnet::tcp_socket_ptr sock = nh.getConnection("_EZDurak");
        if (netp->getConnection(sock))
            players.push_back(netp);
        else
            std::cout << "Error getting connection.. trying again\n";
    }

    int i = 0;
    while (players.size() < numplayers)
    {
        std::string name("AIPlayer");
        name.push_back('1' + i++);
        PlayerPtr p(new AIPlayer(name));
        players.push_back(p);
    }


    Game game(players);
    CLIListener l;
    game.addListener(&l);

    game.run();
}
