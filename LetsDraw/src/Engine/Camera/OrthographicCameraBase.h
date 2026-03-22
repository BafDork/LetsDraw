#pragma once

#include "CameraBase.h"

class GameApp;

class OrthographicCameraBase : public CameraBase
{
public:
    OrthographicCameraBase(GameApp* game,
        float width,
        float height,
        float nearZ = 0.1f,
        float farZ = 100.f);

    void Update(float deltaTime) override {};

    void UpdateProjection();

protected:
    float mWidth;
    float mHeight;
    float mNearZ;
    float mFarZ;
};
