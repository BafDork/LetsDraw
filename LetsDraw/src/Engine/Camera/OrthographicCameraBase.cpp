#include "OrthographicCameraBase.h"

OrthographicCameraBase::OrthographicCameraBase(
    GameApp* game, float width, float height, float nearZ, float farZ)
    : CameraBase(game), mWidth(width), mHeight(height), mNearZ(nearZ), mFarZ(farZ)
{
    UpdateProjection();
}

void OrthographicCameraBase::UpdateProjection()
{
    mProjection = Matrix::CreateOrthographic(mWidth, mHeight, mNearZ, mFarZ);
}
