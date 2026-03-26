#include "CollisionComponent.h"
#include "Engine/Graphics/TransformComponent.h"

BoundingBox CollisionComponent::GetWorldBounds() const
{
    BoundingBox world;
    mLocalBounds.Transform(world, mTransform->GetWorldMatrix());
    return world;
}

BoundingSphere CollisionComponent::GetWorldSphere() const
{
    BoundingSphere sphere = mLocalSphere;

    DirectX::SimpleMath::Matrix world = mTransform->GetWorldMatrix();

    DirectX::SimpleMath::Vector3 scale;
    DirectX::SimpleMath::Quaternion rotation;
    DirectX::SimpleMath::Vector3 translation;

    world.Decompose(scale, rotation, translation);

    sphere.Center = translation;

    float maxScale = scale.x;
    maxScale = std::max(maxScale, scale.y);
    maxScale = std::max(maxScale, scale.z);
    sphere.Radius *= maxScale;

    return sphere;
}

bool CollisionComponent::Intersects(const BoundingBox& other) const
{
    if (mIsBox)
        return GetWorldBounds().Intersects(other);
    else
        return GetWorldSphere().Intersects(other);
}

bool CollisionComponent::Intersects(const BoundingSphere& other) const
{
    if (mIsBox)
        return GetWorldBounds().Intersects(other);
    else
        return GetWorldSphere().Intersects(other);
}

bool CollisionComponent::Intersects(const CollisionComponent& other) const
{
	if ((mIsBox && other.mIsBox) || (!mIsBox && other.mIsBox))
        return Intersects(other.GetWorldBounds());
    else
        return Intersects(other.GetWorldSphere());
}
