#include <iostream>
#include "core/Game.h"
#include "net/NetworkListener.h"
#include "ai/AIPlayer.h"
#include "cli/CLIListener.h"

int main()
{
    std::vector<PlayerPtr> players;
    players.push_back(new NetworkPlayer());
    for (int i = 1; i < 4; i++)
    {
        std::string name("AIPlayer");
        name.push_back('1' + i);
        PlayerPtr p(new AIPlayer(name));
        players.push_back(p);
    }

    Game game(players);
    NetworkListener nl(&game);

    if (nl.getConnection("54321"))
        game.addListener(&nl);
    else
        std::cout << "Error getting connection, Quitting\n";

    CLIListener l(&game);
    game.run();
}
