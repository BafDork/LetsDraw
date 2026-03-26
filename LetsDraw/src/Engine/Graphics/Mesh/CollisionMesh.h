#pragma once

#include <memory>

#include "Engine\GameComponent.h"
#include "Engine\Graphics\Collision\ICollisionProvider.h"
#include "Engine\Graphics\ITransformProvider.h"

class GameApp;
class RenderableComponent;
class TransformComponent;

class CollisionMesh : public GameComponent, public ITransformProvider, public ICollisionProvider
{
public:
    CollisionMesh(
        GameApp* gameApp,
        RenderableComponent* visualMesh,
        ICollisionProvider* collisionMesh);

    TransformComponent* GetTransform() override { return mTransform.get(); }

private:
    RenderableComponent* mVisual;
    ICollisionProvider* mCollision;
    std::unique_ptr<TransformComponent> mTransform;
};
