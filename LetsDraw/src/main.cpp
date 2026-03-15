#include "Game\Game.h"
#include "Game\TransformComponent.h"
#include "Graphics\RenderableComponent.h"

int main()
{
    Game game(800, 800);

    game.AddComponent(std::move(RenderableComponent::CreateQuadComponent(&game)));

    if (!game.Initialize())
        return -1;

    game.Run();

    return 0;
}