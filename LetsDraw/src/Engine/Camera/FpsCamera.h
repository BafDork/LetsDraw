#pragma once

#include "PerspectiveCameraBase.h"

class GameApp;

class FpsCamera : public PerspectiveCameraBase
{
public:
    FpsCamera(GameApp* game) : PerspectiveCameraBase(game) {}

    void Update(float deltaTime) override;

    void SetMouseSensitivity(float sens) { mMouseSensitivity = sens; }
    void SetInvertY(bool invert) { mInvertAxis = invert; }

private:
    float mYaw = 0.f;
    float mPitch = 0.f;
    float mMouseSensitivity = 0.001f;
    bool mInvertAxis = true;
};
