#pragma once

class CollisionComponent;

class ICollisionProvider : public ITransformProvider
{
public:
    virtual ~ICollisionProvider() = default;

    virtual CollisionComponent* GetCollision() = 0;
};
