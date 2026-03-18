#include "CelestialBodyComponent.h"
#include "Engine\GameApp.h"

CelestialBodyComponent::CelestialBodyComponent(
    GameApp* game,
    float radius,
    SphericalComponent* parent,
    float orbitRadius,
    float orbitSpeed)
    : SphericalComponent(game, radius),
    mParent(parent),
    mOrbitRadius(orbitRadius),
    mOrbitSpeed(orbitSpeed)
{
}

void CelestialBodyComponent::Update(float deltaTime)
{
    if (mParent)
    {
        mOrbitAngle += mOrbitSpeed * deltaTime;

        float x = mParent->GetTransform()->GetPosition().x + cosf(mOrbitAngle) * mOrbitRadius;
        float z = mParent->GetTransform()->GetPosition().z + sinf(mOrbitAngle) * mOrbitRadius;

        GetTransform()->SetPosition({ x, 0.0f, z });
    }
}
