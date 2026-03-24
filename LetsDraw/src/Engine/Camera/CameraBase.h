#pragma once

#include <memory>
#include <SimpleMath.h>

#include "Engine/GameComponent.h"
#include "Engine/Graphics/ITransformProvider.h"
#include "Engine/Graphics/TransformComponent.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class GameApp;

class CameraBase : public GameComponent, public ITransformProvider
{
public:
    CameraBase(GameApp* game, std::unique_ptr<TransformComponent> transform = nullptr);

    virtual void Update(float deltaTime) override;

    TransformComponent* GetTransform() { return mTransform.get(); }
    void SetTarget(ITransformProvider* target) { mTarget = target; }

    const Matrix& GetView() const { return mView; }
    const Matrix& GetProjection() const { return mProjection; }

    Vector3 GetForward() const { return mForward; }
    Vector3 GetRight() const { return mRight; }
    Vector3 GetUp() const { return mUp; }

protected:
    std::unique_ptr<TransformComponent> mTransform;
    ITransformProvider* mTarget = nullptr;

    Matrix mView;
    Matrix mProjection;

    Vector3 mForward = Vector3::Forward;
    Vector3 mRight = Vector3::Right;
    Vector3 mUp = Vector3::Up;
};
