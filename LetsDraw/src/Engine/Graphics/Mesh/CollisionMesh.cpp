#include "CollisionMesh.h"
#include "Engine/Graphics/Collision/ICollisionProvider.h"
#include "Engine/Graphics/RenderableComponent.h"
#include "Engine/Graphics/TransformComponent.h"

CollisionMesh::CollisionMesh(
    GameApp* gameApp,
    RenderableComponent* visualMesh,
    ICollisionProvider* collisionMesh)
    : GameComponent(gameApp)
    , mVisual(visualMesh)
    , mCollision(collisionMesh)
{
    mTransform = std::make_unique<TransformComponent>();

    mVisual->GetTransform()->SetParent(mTransform.get());
    mCollision->GetTransform()->SetParent(mTransform.get());
}
