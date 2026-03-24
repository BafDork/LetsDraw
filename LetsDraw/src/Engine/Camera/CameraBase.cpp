#include "CameraBase.h"

CameraBase::CameraBase(GameApp* game, std::unique_ptr<TransformComponent> transform) 
    : GameComponent(game)
{
    if (transform)
        mTransform = std::move(transform);
    else
        mTransform = std::make_unique<TransformComponent>();
}

void CameraBase::Update(float deltaTime)
{
    const Vector3 position = mTransform->GetPosition();

    Vector3 targetPos;
    if (mTarget)
    {
        targetPos = mTarget->GetTransform()->GetPosition();

        mForward = targetPos - position;
        if (mForward.LengthSquared() > 0.f)
        {
            mForward.Normalize();
        }
        else
        {
            mForward = Vector3::Forward;
            targetPos = position + mForward;
        }
    }
    else
    {
        targetPos = position + mForward;
    }

    mView = Matrix::CreateLookAt(position, targetPos, mUp);
}
