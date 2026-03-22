#pragma once

#include "Engine\GameApp.h"

class PlanetariumGame : public GameApp
{
public:
    PlanetariumGame(int width, int height);

protected:
    void CreateCamera() override;
    void OnCreateGame() override;
    void OnUpdate(float delta) override;

private:
    CameraBase* mOrbitCamera = nullptr;
    CameraBase* mFpsCamera = nullptr;

    float moveSpeed = 5.0f;
};
