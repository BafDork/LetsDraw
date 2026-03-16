#include "CollisionComponent.h"
#include "TransformComponent.h"

CollisionComponent::CollisionComponent(
    TransformComponent* transform,
    BoundingBox bounds) : 
    transform(transform),
    localBounds(bounds)
{
}

BoundingBox CollisionComponent::GetWorldBounds() const
{
    BoundingBox world;

    localBounds.Transform(world, transform->GetWorldMatrix());

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
