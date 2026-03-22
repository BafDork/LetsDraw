#pragma once

#include "Engine/GameApp.h"
#include "FpsCamera.h"
#include "Graphics/TransformComponent.h"

void FpsCamera::Update(float deltaTime)
{
    auto input = mGameApp->GetInput();
    float dx = (float) input->GetMouseDeltaX();
    float dy = (float) input->GetMouseDeltaY();
    float invert = mInvertAxis ? -1.f : 1.f;

    mYaw += dx * mMouseSensitivity * invert;
    mPitch += dy * mMouseSensitivity * invert;

    const float limit = DirectX::XM_PIDIV2 - 0.01f;
    if (mPitch > limit) mPitch = limit;
    if (mPitch < -limit) mPitch = -limit;

    mForward.x = cosf(mPitch) * sinf(mYaw);
    mForward.y = sinf(mPitch);
    mForward.z = cosf(mPitch) * cosf(mYaw);
    mForward.Normalize();

    mRight = mForward.Cross(mUp);
    mRight.Normalize();

    mView = Matrix::CreateLookAt(mTransform->GetPosition(), mTransform->GetPosition() + mForward, mUp);
}
