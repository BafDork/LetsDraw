#include "RacketComponent.h"

RacketComponent::RacketComponent(GameApp* gameApp, float width, float height)
    : RenderableComponent(
        gameApp,
        {
            {{ width, height, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, height, 0.0f, 1.0f }, {1,1,1,1}},
            {{ 0.0f, 0.0f, 0.0f, 1.0f }, {1,1,1,1}},
            {{ width, 0.0f, 0.0f, 1.0f }, {1,1,1,1}}
        },
        { 0,1,2, 0,2,3 }
    )
{
    DirectX::BoundingBox box(
        { width / 2.0f, height / 2.0f, 0.0f },
        { width / 2.0f, height / 2.0f, 0.0f }
    );

    mCollision = std::make_unique<CollisionComponent>(GetTransform(), box);
}
