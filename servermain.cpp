#include <iostream>
#include "core/Game.h"
#include "net/NetworkListener.h"
#include "net/NetworkPlayer.h"
#include "ai/AIPlayer.h"
#include "cli/CLIListener.h"

int main()
{
    std::vector<PlayerPtr> players;
    boost::shared_ptr<NetworkPlayer> netp(new NetworkPlayer());
    players.push_back(netp);
    for (int i = 0; i < 3; i++)
    {
        std::string name("AIPlayer");
        name.push_back('1' + i);
        PlayerPtr p(new AIPlayer(name));
        players.push_back(p);
    }

    Game game(players);
    CLIListener l;
    game.addListener(&l);

    if (netp->getConnection("54321"))
        game.run();
    else
        std::cout << "Error getting connection, Quitting\n";

}
