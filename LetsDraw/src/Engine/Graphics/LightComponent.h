#pragma once

#include "Engine/GameComponent.h"
#include "ITransformProvider.h"
#include "TransformComponent.h"

#include <SimpleMath.h>
#include <memory>

using DirectX::SimpleMath::Vector3;

class GameApp;

class LightComponent : public GameComponent, public ITransformProvider
{
public:
    LightComponent(GameApp* game);

    void Initialize() override {}

    TransformComponent* GetTransform() { return mTransform.get(); }

    void SetDirection(const Vector3& dir);
    Vector3 GetDirection() const;

    void SetIntensity(float intensity) { mIntensity = intensity; }
    float GetIntensity() const { return mIntensity; }

private:
    std::unique_ptr<TransformComponent> mTransform;

    Vector3 mDirection = { 0.3f, -1.0f, 0.2f };
    float mIntensity = 1.0f;
};