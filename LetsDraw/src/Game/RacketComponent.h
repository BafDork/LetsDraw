#pragma once

#include "Game\CollisionComponent.h"
#include "Graphics\RenderableComponent.h"

class RacketComponent : public RenderableComponent
{
public:
    RacketComponent(Game* game, float width, float height);
    ~RacketComponent() = default;

    CollisionComponent* GetCollision() const
    {
        return collision.get();
    }

    void SetCollision(std::unique_ptr<CollisionComponent> col)
    {
        collision = std::move(col);
    }

private:
    std::unique_ptr<CollisionComponent> collision;
};
