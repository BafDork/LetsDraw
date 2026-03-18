#include "BallComponent.h"
#include "Engine\GameApp.h"

BallComponent::BallComponent(GameApp* gameApp, float size)
    : RenderableComponent(gameApp),
    mSize(size)
{
    DirectX::BoundingBox box(
        { mSize / 2.0f, mSize / 2.0f, 0.0f },
        { mSize / 2.0f, mSize / 2.0f, 0.0f }
    );

    mCollision = std::make_unique<CollisionComponent>(GetTransform(), box);
}

void BallComponent::Update(float deltaTime)
{
    GetTransform()->Translate({ mVelocity.x * deltaTime, mVelocity.y * deltaTime, 0.0f });
}

void BallComponent::GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices)
{
    outVertices = {
            {{ mSize, mSize, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, mSize, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, 0.0f, 0.0f, 1.0f }, {1,1,1,1}},
            {{ mSize, 0.0f, 0.0f, 1.0f }, {1,1,1,1}}
    };
    outIndices = { 0,1,2, 0,2,3 };
}
