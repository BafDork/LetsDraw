#include "CameraComponent.h"
#include "Engine\GameApp.h"

CameraComponent::CameraComponent(GameApp* game, CameraProjection type)
    : GameComponent(game), mType(type)
{
}

void CameraComponent::Initialize()
{
    UpdateProjection();
}

void CameraComponent::Update(float)
{
    auto input = mGameApp->GetInput();

    float dx = (float) input->GetMouseDeltaX();
    float dy = (float) input->GetMouseDeltaY();
    float invert = mInvertY ? -1.0f : 1.0f;

    mYaw += dx * mMouseSensitivity * invert;
    mPitch += dy * mMouseSensitivity * invert;

    const float limit = DirectX::XM_PIDIV2 - 0.01f;

    if (mPitch > limit)  mPitch = limit;
    if (mPitch < -limit) mPitch = -limit;

    mForward.x = cosf(mPitch) * sinf(mYaw);
    mForward.y = sinf(mPitch);
    mForward.z = cosf(mPitch) * cosf(mYaw);

    mForward.Normalize();

    mTarget = mPosition + mForward;

    mView = Matrix::CreateLookAt(
        mPosition,
        mTarget,
        Vector3::UnitY);
}

void CameraComponent::SetAspect(float aspect)
{
    mAspect = aspect;
    UpdateProjection();
}

void CameraComponent::SetPerspective(float fov, float aspect, float nearZ, float farZ)
{
    mFov = fov; mAspect = aspect; mNearZ = nearZ; mFarZ = farZ;
    mType = CameraProjection::Perspective;
    UpdateProjection();
}

void CameraComponent::SetOrthographic(float width, float height, float nearZ, float farZ)
{
    mOrthoWidth = width; mOrthoHeight = height; mNearZ = nearZ; mFarZ = farZ;
    mType = CameraProjection::Orthographic;
    UpdateProjection();
}

void CameraComponent::UpdateProjection()
{
    if (mType == CameraProjection::Perspective)
    {
        mProjection = Matrix::CreatePerspectiveFieldOfView(
            DirectX::XMConvertToRadians(mFov),
            mAspect,
            mNearZ,
            mFarZ);
    }
    else
    {
        mProjection = Matrix::CreateOrthographic(
            mOrthoWidth,
            mOrthoHeight,
            mNearZ,
            mFarZ);
    }
}

const Vector3& CameraComponent::GetRight()
{ 
    mRight = mForward.Cross(mUp);
	mRight.Normalize();
	return mRight;
}

void CameraComponent::Move(const Vector3& delta)
{
    mPosition.x += delta.x;
    mPosition.y += delta.y;
    mPosition.z += delta.z;
}
