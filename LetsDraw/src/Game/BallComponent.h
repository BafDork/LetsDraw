#pragma once

#include "Game\CollisionComponent.h"
#include "Graphics\RenderableComponent.h"

#include <DirectXMath.h>

using DirectX::XMFLOAT3;

class Game;

class BallComponent : public RenderableComponent
{
public:
    BallComponent(Game* game, float size);
    ~BallComponent() = default;

    CollisionComponent* GetCollision() const { return collision.get(); }

    void SetVelocity(const XMFLOAT3& vel) { velocity = vel; }
    XMFLOAT3 GetVelocity() const { return velocity; }

    void Update(float deltaTime) override;

private:
    std::unique_ptr<CollisionComponent> collision;
    XMFLOAT3 velocity;
};