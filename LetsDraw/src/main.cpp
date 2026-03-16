#include "Game\Game.h"

int main()
{
    Game game(800, 800);

    if (!game.Initialize())
        return -1;

    game.Run();

    return 0;
}