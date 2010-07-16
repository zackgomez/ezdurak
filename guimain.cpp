#include <ctime>
#include "gui/GUIImpl.h"
#include "core/Game.h"
#include "core/PlayerImpl.h"
#include "ai/AIPlayer.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    GUIImpl g;
    g.run();
}
