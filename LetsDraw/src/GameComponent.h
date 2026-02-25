#pragma once

class Game;

class GameComponent
{
public:
    explicit GameComponent(Game* game)
        : game(game)
    {
    }

    virtual ~GameComponent() = default;

    virtual void Initialize() {}
    virtual void Reload() {}
    virtual void Update(float deltaTime) {}
    virtual void Draw() {}
    virtual void DestroyResources() {}

protected:
    Game* game;
};
