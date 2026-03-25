#pragma once

#include "Engine\GameApp.h"

class SphericalComponent;

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
	SphericalComponent* mBall = nullptr;

    float moveSpeed = 3.0f;
	float mBallRadius = 0.5f;
};
