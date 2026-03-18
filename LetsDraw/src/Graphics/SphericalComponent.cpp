#include "SphericalComponent.h"
#include "Engine\GameApp.h"
#include <cmath>

SphericalComponent::SphericalComponent(GameApp* gameApp, float radius, int slices, int stacks) :
    RenderableComponent(gameApp),
    mRadius(radius), mSlices(slices), mStacks(stacks)
{
}

void SphericalComponent::GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices)
{
    outVertices.clear();
    outIndices.clear();

    for (int stack = 0; stack <= mStacks; ++stack)
    {
        float phi = DirectX::XM_PI * stack / mStacks;
        for (int slice = 0; slice <= mSlices; ++slice)
        {
            float theta = DirectX::XM_2PI * slice / mSlices;

            float x = mRadius * sinf(phi) * cosf(theta);
            float y = mRadius * cosf(phi);
            float z = mRadius * sinf(phi) * sinf(theta);

            Vertex v{};
            v.position = Vector4(x, y, z, 1.0f);
            v.color = mColor;
            outVertices.push_back(v);
        }
    }

    for (int stack = 0; stack < mStacks; ++stack)
    {
        for (int slice = 0; slice < mSlices; ++slice)
        {
            int first = stack * (mSlices + 1) + slice;
            int second = first + mSlices + 1;

            outIndices.push_back(first);
            outIndices.push_back(second);
            outIndices.push_back(first + 1);

            outIndices.push_back(second);
            outIndices.push_back(second + 1);
            outIndices.push_back(first + 1);
        }
    }
}
