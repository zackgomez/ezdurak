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

    std::set<NetworkClient::Connection> conns;

    gamefinder.listen();
    int tries = 0;
    while (conns.empty())
    {
        conns = gamefinder.getConnectionList();
        sleep(1);
        if (++tries > 5)
        {
            std::cout << "Unable to find game after 5 tries\n";
            exit(0);
        }
    }
    gamefinder.ignore();


    std::set<NetworkClient::Connection>::iterator it = conns.begin();
    int i = 1;
    for (; it != conns.end(); it++)
    {
        std::cout << "Connection " << i << ": " << it->addr << " on " << it->port << " : " << it->aux << '\n';
    }

    it = conns.begin();
    kissnet::tcp_socket_ptr sock = kissnet::tcp_socket::create();
    sock->connect(it->addr, it->port);

    // Use the socket and play a game
    NetworkGame game;
    game.addPlayer(localPlayer);
    CLIListener listener;
    game.addListener(&listener);

    if(game.connectTo(sock))
        game.run();
    else
        std::cout << "Unable to connect, quiting\n";
}
