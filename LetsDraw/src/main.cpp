#include "Planetarium\PlanetariumGame.h"

int main()
{
    PlanetariumGame game(800, 800);

    if (!game.Initialize())
        return -1;

    game.Run();

    return 0;
}