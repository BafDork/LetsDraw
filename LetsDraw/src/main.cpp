#include "PongGame\PongGame.h"

int main()
{
    PongGame game(800, 800);

    if (!game.Initialize())
        return -1;

    game.Run();

    return 0;
}