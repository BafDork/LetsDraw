#pragma once

#include "Engine\Graphics\Collision\CollisionComponent.h"
#include "Engine\Graphics\Collision\ICollisionProvider.h"
#include "Engine\Graphics\RenderableComponent.h"

using DirectX::SimpleMath::Vector4;

class GameApp;

class SphericalComponent : public RenderableComponent, public ICollisionProvider
{
public:
    SphericalComponent(GameApp* gameApp, float radius = 1.0f, int slices = 16, int stacks = 16);

	void SetColor(const Vector4& color) { mColor = color; }

    CollisionComponent* GetCollision() override { return mCollision.get(); }

protected:
    void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) override;

private:
    float mRadius;
    int mSlices;
    int mStacks;

	Vector4 mColor{ 1,1,1,1 };

    std::unique_ptr<CollisionComponent> mCollision;
};