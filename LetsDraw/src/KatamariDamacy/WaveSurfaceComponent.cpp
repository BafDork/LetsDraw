#include "WaveSurfaceComponent.h"
#include "Engine/GameApp.h"
#include <cmath>

WaveSurfaceComponent::WaveSurfaceComponent(GameApp* gameApp, float width, float depth, int xSegments, int zSegments)
    : RenderableComponent(gameApp),
    mWidth(width),
    mDepth(depth),
    mXSegments(xSegments),
    mZSegments(zSegments)
{
}

void WaveSurfaceComponent::GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices)
{
    outVertices.clear();
    outIndices.clear();

    float dx = mWidth / mXSegments;
    float dz = mDepth / mZSegments;

    float startX = -mWidth * 0.5f;
    float startZ = -mDepth * 0.5f;

    for (int z = 0; z <= mZSegments; ++z)
    {
        for (int x = 0; x <= mXSegments; ++x)
        {
            float posX = startX + x * dx;
            float posZ = startZ + z * dz;

            float posY = GetHeight(posX, posZ);

            Vertex v{};
            v.position = { posX, posY, posZ, 1.0f };
            v.color = mColor;

            outVertices.push_back(v);
        }
    }

    for (int z = 0; z < mZSegments; ++z)
    {
        for (int x = 0; x < mXSegments; ++x)
        {
            int first = z * (mXSegments + 1) + x;
            int second = first + mXSegments + 1;

            outIndices.push_back(first);
            outIndices.push_back(second);
            outIndices.push_back(first + 1);

            outIndices.push_back(second);
            outIndices.push_back(second + 1);
            outIndices.push_back(first + 1);
        }
    }
}

float WaveSurfaceComponent::GetHeight(float x, float z) const
{
    return
        sinf(x * mFrequency) *
        cosf(z * mFrequency) *
        mAmplitude;
}
