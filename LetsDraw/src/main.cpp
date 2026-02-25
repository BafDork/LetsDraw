#include "Game.h"
#include "TriangleComponent.h"

int main()
{
    Game game(800, 800);

    game.AddComponent(std::make_unique<TriangleComponent>(&game));

    if (!game.Initialize())
        return -1;

    game.Run();

    return 0;
}