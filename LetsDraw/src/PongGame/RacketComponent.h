#pragma once

#include "Graphics\CollisionComponent.h"
#include "Graphics\RenderableComponent.h"

class GameApp;

class RacketComponent : public RenderableComponent
{
public:
    RacketComponent(GameApp* gameApp, float width, float height);

    CollisionComponent* GetCollision() const { return mCollision.get(); }
    void SetCollision(std::unique_ptr<CollisionComponent> collision) { mCollision = std::move(collision); }

private:
    std::unique_ptr<CollisionComponent> mCollision;
};
