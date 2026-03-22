#include "PerspectiveCameraBase.h"

PerspectiveCameraBase::PerspectiveCameraBase(
    GameApp* game, float fov, float aspect, float nearZ, float farZ)
    : CameraBase(game), mFov(fov), mAspect(aspect), mNearZ(nearZ), mFarZ(farZ)
{
    UpdateProjection();
}

void PerspectiveCameraBase::UpdateProjection()
{
    mProjection = Matrix::CreatePerspectiveFieldOfView(
        DirectX::XMConvertToRadians(mFov),
        mAspect,
        mNearZ,
        mFarZ);
}

void PerspectiveCameraBase::SetAspect(float aspect)
{
    mAspect = aspect;
    UpdateProjection();
}
