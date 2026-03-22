#pragma once

#include "Engine\GameComponent.h"
#include <SimpleMath.h>

using DirectX::SimpleMath::Vector4;

class GameApp;
class TransformComponent;

class CelestialMotionComponent : public GameComponent
{
public:
    CelestialMotionComponent(
        GameApp* game,
        TransformComponent* transform,
        TransformComponent* parentTransform,
        float orbitRadius = 0.0f,
        float orbitSpeed = 0.0f,
        float selfRotationSpeed = 0.0f);

    void Update(float deltaTime) override;

private:
    TransformComponent* mTransform;
    TransformComponent* mParentTransform;

    float mOrbitRadius;
    float mOrbitSpeed;
    float mOrbitAngle = 0.f;

    float mSelfRotationSpeed;
    float mSelfRotationAngle = 0.f;
};
