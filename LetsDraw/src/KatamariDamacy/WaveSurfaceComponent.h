#pragma once

#include "Engine/Graphics/RenderableComponent.h"

using DirectX::SimpleMath::Vector4;

class GameApp;

class WaveSurfaceComponent : public RenderableComponent
{
public:
    WaveSurfaceComponent(
        GameApp* gameApp,
        float width = 50.f,
        float depth = 50.f,
        int xSegments = 50,
        int zSegments = 50);

    float GetHeight(float x, float z) const;

    void SetColor(const Vector4& color) { mColor = color; }

protected:
    void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) override;

private:
    float mWidth;
    float mDepth;

    int mXSegments;
    int mZSegments;

    float mAmplitude = 1.0f;
    float mFrequency = 0.5f;

    Vector4 mColor{ 1,1,1,1 };
};
