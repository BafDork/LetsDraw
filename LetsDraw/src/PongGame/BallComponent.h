#pragma once

#include "Graphics\CollisionComponent.h"
#include "Graphics\RenderableComponent.h"

#include <DirectXMath.h>

using DirectX::XMFLOAT3;

class GameApp;

class BallComponent : public RenderableComponent
{
public:
    BallComponent(GameApp* gameApp, float size);

    void Update(float deltaTime) override;

    XMFLOAT3 GetVelocity() const { return mVelocity; }
    void SetVelocity(const XMFLOAT3& vel) { mVelocity = vel; }
    CollisionComponent* GetCollision() const { return mCollision.get(); }

private:
    XMFLOAT3 mVelocity;
    std::unique_ptr<CollisionComponent> mCollision;
};