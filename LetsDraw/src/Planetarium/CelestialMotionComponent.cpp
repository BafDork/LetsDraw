#include "CelestialMotionComponent.h"
#include "Graphics/ITransformProvider.h"
#include "Graphics/RenderableComponent.h"
#include "Graphics/TransformComponent.h"

CelestialMotionComponent::CelestialMotionComponent(
    GameApp* game,
    ITransformProvider* owner,
    ITransformProvider* parent,
    float orbitRadius,
    float orbitSpeed,
    float selfRotationSpeed)
    : GameComponent(game), 
    mOwner(owner),
    mParent(parent),
    mOrbitRadius(orbitRadius),
    mOrbitSpeed(orbitSpeed),
    mSelfRotationSpeed(selfRotationSpeed)
{
}

void CelestialMotionComponent::Update(float deltaTime)
{
    if (mParent)
    {
        mOrbitAngle += mOrbitSpeed * deltaTime;

        auto parentPos = mParent->GetTransform()->GetPosition();

        float x = parentPos.x + cosf(mOrbitAngle) * mOrbitRadius;
        float z = parentPos.z + sinf(mOrbitAngle) * mOrbitRadius;

        mOwner->GetTransform()->SetPosition({ x, 0.f, z });
    }

    mSelfRotationAngle += mSelfRotationSpeed * deltaTime;

    mOwner->GetTransform()->SetRotation({ 0.f, mSelfRotationAngle, 0.f });
}
