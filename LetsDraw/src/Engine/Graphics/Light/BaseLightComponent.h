#pragma once

#include <memory>
#include <SimpleMath.h>

#include "Engine/GameComponent.h"
#include "Engine/Graphics/ITransformProvider.h"
#include "Engine/Graphics/TransformComponent.h"

using DirectX::SimpleMath::Vector3;

class GameApp;

enum class LightType
{
    Directional = 0,
    Point = 1
};

class BaseLightComponent : public GameComponent, public ITransformProvider
{
public:
    BaseLightComponent(GameApp* game, LightType type) :
        GameComponent(game),
        mTransform(std::make_unique<TransformComponent>()),
        mType(type) {}

    void Initialize() override {}
    void Update(float deltaTime) override {}

    TransformComponent* GetTransform() { return mTransform.get(); }

    void SetIntensity(float intensity) { mIntensity = intensity; }
    float GetIntensity() const { return mIntensity; }

    void SetColor(const Vector3& color) { mColor = color; }
    Vector3 GetColor() const { return mColor; }

    LightType GetType() const { return mType; }

protected:
    std::unique_ptr<TransformComponent> mTransform;

    float mIntensity = 1.0f;
    Vector3 mColor = { 1.0f, 1.0f, 1.0f };

    LightType mType;
};
