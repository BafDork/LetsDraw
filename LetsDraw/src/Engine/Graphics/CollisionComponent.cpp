#include "CollisionComponent.h"
#include "TransformComponent.h"

CollisionComponent::CollisionComponent(
    TransformComponent* transform,
    BoundingBox localBounds) :
    mTransform(transform),
    mLocalBounds(localBounds)
{
}

BoundingBox CollisionComponent::GetWorldBounds() const
{
    BoundingBox world;

    mLocalBounds.Transform(world, mTransform->GetWorldMatrix());

    return world;
}

bool CollisionComponent::Intersects(const CollisionComponent& other) const
{
    return Intersects(other.GetWorldBounds());
}

bool CollisionComponent::Intersects(const BoundingBox& other) const
{
    return GetWorldBounds().Intersects(other);
}
