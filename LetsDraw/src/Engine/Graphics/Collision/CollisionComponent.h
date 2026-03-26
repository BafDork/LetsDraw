#pragma once

#define NOMINMAX
#include <DirectXCollision.h>
#include <memory>

#include "Engine\Graphics\ITransformProvider.h"

using DirectX::BoundingBox;
using DirectX::BoundingSphere;

class TransformComponent;

class CollisionComponent : public ITransformProvider
{
public:
    CollisionComponent(TransformComponent* transform, BoundingBox localBounds)
        : mTransform(transform), mLocalBounds(localBounds), mIsBox(true)
    {
    }
    CollisionComponent(TransformComponent* transform, BoundingSphere localSphere)
        : mTransform(transform), mLocalSphere(localSphere), mIsBox(false)
    {
    }

    bool Intersects(const CollisionComponent& other) const;
    bool Intersects(const BoundingBox& other) const;
    bool Intersects(const BoundingSphere& other) const;

    TransformComponent* GetTransform() { return mTransform; }

private:
    BoundingBox GetWorldBounds() const;
    BoundingSphere GetWorldSphere() const;

private:
    TransformComponent* mTransform;

    BoundingBox mLocalBounds{};
    BoundingSphere mLocalSphere{};

    bool mIsBox{ true };
};