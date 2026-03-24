#pragma once

#include "CameraBase.h"

class GameApp;

class PerspectiveCameraBase : public CameraBase
{
public:
    PerspectiveCameraBase(GameApp* game,
        float fov = 70.f,
        float aspect = 16.f / 9.f,
        float nearZ = 0.1f,
        float farZ = 100.f);

    void UpdateProjection();

    void SetAspect(float aspect);

    float GetFov() const { return mFov; }

protected:
    float mFov;
    float mAspect;
    float mNearZ;
    float mFarZ;
};
