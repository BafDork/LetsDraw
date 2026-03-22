#include "CelestialMotionComponent.h"
#include "Graphics/RenderableComponent.h"
#include "Graphics/TransformComponent.h"

CelestialMotionComponent::CelestialMotionComponent(
    GameApp* game,
    TransformComponent* transform,
    TransformComponent* parentTransform,
    float orbitRadius,
    float orbitSpeed,
    float selfRotationSpeed)
    : GameComponent(game), 
    mTransform(transform),
    mParentTransform(parentTransform),
    mOrbitRadius(orbitRadius),
    mOrbitSpeed(orbitSpeed),
    mSelfRotationSpeed(selfRotationSpeed)
{
}

void CelestialMotionComponent::Update(float deltaTime)
{
    if (mParentTransform)
    {
        mOrbitAngle += mOrbitSpeed * deltaTime;

        auto parentPos = mParentTransform->GetPosition();

        float x = parentPos.x + cosf(mOrbitAngle) * mOrbitRadius;
        float z = parentPos.z + sinf(mOrbitAngle) * mOrbitRadius;

        mTransform->SetPosition({ x, 0.f, z });
    }

    mSelfRotationAngle += mSelfRotationSpeed * deltaTime;

    mTransform->SetRotation({ 0.f, mSelfRotationAngle, 0.f });
}
