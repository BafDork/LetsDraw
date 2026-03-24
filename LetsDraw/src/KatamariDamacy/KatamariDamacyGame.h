#pragma once

#include "Engine\GameApp.h"

class KatamariDamacyGame : public GameApp
{
public:
    KatamariDamacyGame(int width, int height);

protected:
    void CreateCamera() override;
    void OnCreateGame() override;
    void OnUpdate(float delta) override;

private:
    CameraBase* mFpsCamera = nullptr;

    float moveSpeed = 1.0f;
};
