#include "BoxComponent.h"
#include "Engine/GameApp.h"
#include "Graphics/Vertex.h"

BoxComponent::BoxComponent(GameApp* gameApp, float width, float height, float depth)
    : RenderableComponent(gameApp), mWidth(width), mHeight(height),mDepth(depth)
{
}

void BoxComponent::GetMesh(std::vector<Vertex>& outVertices,
    std::vector<uint32_t>& outIndices)
{
    outVertices.clear();
    outIndices.clear();

    float w = mWidth * 0.5f;
    float h = mHeight * 0.5f;
    float d = mDepth * 0.5f;

    Vector3 positions[] =
    {
        {-w,-h,-d}, { w,-h,-d}, { w, h,-d}, {-w, h,-d},
        {-w,-h, d}, { w,-h, d}, { w, h, d}, {-w, h, d}
    };

    for (auto& p : positions)
    {
        Vertex v{};
        v.position = Vector4(p.x, p.y, p.z, 1.0f);
        v.color = mColor;
        outVertices.push_back(v);
    }

    uint32_t indices[] =
    {
        // back
        0,1,2,
        0,2,3,

        // front
        4,6,5,
        4,7,6,

        // left
        4,0,3,
        4,3,7,

        // right
        1,5,6,
        1,6,2,

        // top
        3,2,6,
        3,6,7,

        // bottom
        4,5,1,
        4,1,0
    };

    outIndices.insert(outIndices.end(), std::begin(indices), std::end(indices));
}
