#include "BallComponent.h"
#include "Engine\GameApp.h"

BallComponent::BallComponent(GameApp* gameApp, float size)
    : RenderableComponent(
        gameApp,
        {
            {{ size, size, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, size, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, 0.0f, 0.0f, 1.0f }, {1,1,1,1}},
            {{ size, 0.0f, 0.0f, 1.0f }, {1,1,1,1}}
        },
        { 0,1,2, 0,2,3 }
    ),
    mVelocity({ 0.5f, 0.3f, 0.0f })
{
    DirectX::BoundingBox box(
        { size / 2.0f, size / 2.0f, 0.0f },
        { size / 2.0f, size / 2.0f, 0.0f }
    );

    mCollision = std::make_unique<CollisionComponent>(GetTransform(), box);
}

void BallComponent::Update(float deltaTime)
{
    GetTransform()->Translate({ mVelocity.x * deltaTime, mVelocity.y * deltaTime, 0.0f });
}
