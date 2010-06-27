#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "CLIListener.h"

int main()
{
    srand(time(NULL));
    Game game(4);
    CLIListener listener(&game);

    game.run();

    return 0;
}
