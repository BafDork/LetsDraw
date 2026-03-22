#pragma once

#include "RenderableComponent.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

class GameApp;

class BoxComponent : public RenderableComponent
{
public:
    BoxComponent(GameApp* gameApp,
        float width = 1.0f,
        float height = 1.0f,
        float depth = 1.0f);

    void SetColor(const Vector4& color) { mColor = color; }

protected:
    void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) override;

private:
    float mWidth;
    float mHeight;
    float mDepth;

    Vector4 mColor{ 1,1,1,1 };
};
