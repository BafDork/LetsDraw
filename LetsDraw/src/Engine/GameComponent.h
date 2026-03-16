#pragma once

class GameApp;

class GameComponent
{
public:
    explicit GameComponent(GameApp* gameApp)
        : mGameApp(gameApp)
    {
    }

    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Draw() {}
    virtual void DestroyResources() {}

protected:
    GameApp* mGameApp;
};
