#pragma once

#include "Graphics\SphericalComponent.h"

using DirectX::SimpleMath::Vector4;

class CelestialBodyComponent : public SphericalComponent
{
public:
    CelestialBodyComponent(
        GameApp* game,
        float radius,
        SphericalComponent* parent = nullptr,
        float orbitRadius = 0.0f,
        float orbitSpeed = 0.0f);

    void Update(float deltaTime) override;

private:
    SphericalComponent* mParent;

    float mOrbitRadius;
    float mOrbitSpeed;
    float mOrbitAngle = 0.0f;

    Vector4 mColor;
};