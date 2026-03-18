#include "RacketComponent.h"

RacketComponent::RacketComponent(GameApp* gameApp, float width, float height)
    : RenderableComponent(gameApp),
	mWidth(width), mHeight(height)
{
    DirectX::BoundingBox box(
        { mWidth / 2.0f, mHeight / 2.0f, 0.0f },
        { mWidth / 2.0f, mHeight / 2.0f, 0.0f }
    );

    mCollision = std::make_unique<CollisionComponent>(GetTransform(), box);
}

void RacketComponent::GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices)
{
    outVertices = {
            {{ mWidth, mHeight, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, mHeight, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, 0.0f, 0.0f, 1.0f }, {1,1,1,1}},
            {{ mWidth, 0.0f, 0.0f, 1.0f }, {1,1,1,1}}
    };
    outIndices = { 0,1,2, 0,2,3 };
}
