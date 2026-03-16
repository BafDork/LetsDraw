#pragma once

#include "BallComponent.h"
#include "Engine\GameApp.h"
#include "RacketComponent.h"
#include <DirectXCollision.h>

using DirectX::BoundingBox;

class PongGame : public GameApp
{
public:
    PongGame(int width, int height);

protected:
    void OnCreateGame() override;
    void OnUpdate(float dt) override;

private:
    void HandleInput();
    void HandleCollisions();
    void HitBall(bool isLeftRacket);
    void ResetBall(bool leftLost);
	void PrintScore();

private:
    RacketComponent* mLeftRacket{};
    RacketComponent* mRightRacket{};
    BallComponent* mBall{};

    BoundingBox mTopWall;
    BoundingBox mBottomWall;
    BoundingBox mLeftWall;
    BoundingBox mRightWall;

    int mLeftScore = 0;
    int mRightScore = 0;

    float mBallSpeedMultiplier = 1.2f;
};
