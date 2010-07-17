#include <ctime>
#include <cstdlib>
#include "gui/GUIApp.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    GUIApp g;
    g.run();

	return 0;
}
