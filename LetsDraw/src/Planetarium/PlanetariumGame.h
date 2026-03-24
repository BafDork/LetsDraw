#pragma once

#include "Engine\GameApp.h"

class PerspectiveCameraBase;

class PlanetariumGame : public GameApp
{
public:
    PlanetariumGame(int width, int height);

protected:
    void CreateCamera() override;
    void OnCreateGame() override;
    void OnUpdate(float delta) override;

private:
    CameraBase* mFpsCamera = nullptr;
    PerspectiveCameraBase* mSunCamera = nullptr;
    PerspectiveCameraBase* mEarthCamera = nullptr;

    float moveSpeed = 5.0f;
};
