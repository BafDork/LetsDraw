#pragma once

#include "RenderableComponent.h"

using DirectX::SimpleMath::Vector4;

class GameApp;

class SphericalComponent : public RenderableComponent
{
public:
    SphericalComponent(GameApp* gameApp, float radius = 1.0f, int slices = 16, int stacks = 16);

	void SetColor(const Vector4& color) { mColor = color; }

protected:
    void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) override;

private:
    float mRadius;
    int mSlices;
    int mStacks;

	Vector4 mColor{ 1,1,1,1 };
};