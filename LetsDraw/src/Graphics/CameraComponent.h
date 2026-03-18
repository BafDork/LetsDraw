#pragma once

#include <SimpleMath.h>

#include "Engine\GameComponent.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class GameApp;

enum class CameraProjection
{
    Perspective,
    Orthographic
};

class CameraComponent : public GameComponent
{
public:
    CameraComponent(GameApp* gameApp, CameraProjection type);

    void Initialize() override;
    void Update(float deltaTime) override;

    void SetPosition(const Vector3& pos) { mPosition = pos; }
    void SetTarget(const Vector3& target) { mTarget = target; }
    void SetProjectionType(CameraProjection type) { mType = type; }

    const Matrix& GetView() const { return mView; }
    const Matrix& GetProjection() const { return mProjection; }
    const Vector3& GetForward() const { return mForward; }
    const Vector3& GetRight();

    void Move(const Vector3& delta);

    void SetPerspective(float fov, float aspect, float nearZ, float farZ);
    void SetOrthographic(float width, float height, float nearZ, float farZ);
    void SetAspect(float aspect);
    void SetInvertY(bool value) { mInvertY = value; }

private:
    void UpdateProjection();

private:
    CameraProjection mType;

    Vector3 mPosition;
    Vector3 mTarget;

    Vector3 mForward;
    Vector3 mRight;
    Vector3 mUp = Vector3::UnitY;

    Matrix mView;
    Matrix mProjection;

    // Perspective params
    float mFov = 70.f;
    float mAspect = 16.f / 9.f;
    float mNearZ = 0.1f;
    float mFarZ = 100.f;

    // Orthographic params
    float mOrthoWidth = 10.f;
    float mOrthoHeight = 10.f;

    float mYaw = 0.0f;
    float mPitch = 0.0f;

    float mMouseSensitivity = 0.001f;
    bool mInvertY = true;
};
