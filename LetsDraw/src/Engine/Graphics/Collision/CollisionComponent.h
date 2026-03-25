#pragma once

#include <DirectXCollision.h>

using DirectX::BoundingBox;

class TransformComponent;

class CollisionComponent
{
public:
    CollisionComponent(
        TransformComponent* transform,
        BoundingBox localBounds);

    bool Intersects(const CollisionComponent& other) const;
    bool Intersects(const BoundingBox& other) const;

    BoundingBox GetWorldBounds() const;

private:
    TransformComponent* mTransform;
    BoundingBox mLocalBounds;
};