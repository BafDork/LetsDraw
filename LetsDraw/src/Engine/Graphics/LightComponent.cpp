#include "LightComponent.h"

LightComponent::LightComponent(GameApp* game)
    : GameComponent(game),
    mTransform(std::make_unique<TransformComponent>())
{
    mDirection.Normalize();
}

void LightComponent::SetDirection(const Vector3& dir)
{
    mDirection = dir;
    mDirection.Normalize();
}

Vector3 LightComponent::GetDirection() const
{
    return mDirection;
}
