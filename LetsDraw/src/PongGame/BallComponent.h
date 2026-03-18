#pragma once

#include "Graphics\CollisionComponent.h"
#include "Graphics\RenderableComponent.h"

#include <SimpleMath.h>

using DirectX::SimpleMath::Vector3;

class GameApp;

class BallComponent : public RenderableComponent
{
public:
    BallComponent(GameApp* gameApp, float size);

    void Update(float deltaTime) override;

    Vector3 GetVelocity() const { return mVelocity; }
    void SetVelocity(const Vector3& vel) { mVelocity = vel; }
    CollisionComponent* GetCollision() const { return mCollision.get(); }

protected:
    void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) override;

private:
    Vector3 mVelocity{ 0.0f, 0.0f, 0.0f };
    std::unique_ptr<CollisionComponent> mCollision;

    float mSize;
};