#include "Game.h"
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(NULL));
    Game game(4);

    game.run();

    return 0;
}
