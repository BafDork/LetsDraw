#pragma once

class CollisionComponent;

class ICollisionProvider
{
public:
    virtual ~ICollisionProvider() = default;

    virtual CollisionComponent* GetCollision() = 0;
};
