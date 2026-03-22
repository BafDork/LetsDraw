#pragma once

#include <SimpleMath.h>
#include <memory>

#include "Engine/GameComponent.h"
#include "Graphics/TransformComponent.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class GameApp;

class CameraBase : public GameComponent
{
public:
    CameraBase(GameApp* game, std::unique_ptr<TransformComponent> transform = nullptr) : GameComponent(game)
    {
        if (transform)
            mTransform = std::move(transform);
        else
            mTransform = std::make_unique<TransformComponent>();
    }

    virtual void Update(float deltaTime) override = 0;

    TransformComponent* GetTransform() { return mTransform.get(); }

    const Matrix& GetView() const { return mView; }
    const Matrix& GetProjection() const { return mProjection; }
    const Vector3& GetForward() const { return mForward; }
    const Vector3& GetRight() const { return mRight; }

protected:
    std::unique_ptr<TransformComponent> mTransform;

    Vector3 mForward;
    Vector3 mRight;
    Vector3 mUp = Vector3::UnitY;

    Matrix mView;
    Matrix mProjection;
};
