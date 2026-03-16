#include <iostream>

#include "PongGame.h"
#include "Window\Keys.h"

PongGame::PongGame(int width, int height)
    : GameApp(width, height)
{
}

void PongGame::OnCreateGame()
{
    float halfWidth = 1.0f;
    float halfHeight = 1.0f;
    float wallThickness = 0.01f;

    mTopWall = BoundingBox(
        { 0.f, halfHeight, 0.f },
        { halfWidth, wallThickness, 0.f }
    );

    mBottomWall = BoundingBox(
        { 0.f, -halfHeight, 0.f },
        { halfWidth, wallThickness, 0.f }
    );

    mLeftWall = BoundingBox(
        { -halfWidth, 0.f, 0.f },
        { wallThickness, halfHeight, 0.f }
    );

    mRightWall = BoundingBox(
        { halfWidth, 0.f, 0.f },
        { wallThickness, halfHeight, 0.f }
    );

    auto left = std::make_unique<RacketComponent>(this, 0.1f, 0.4f);
    mLeftRacket = left.get();
    mLeftRacket->GetTransform()->SetPosition({ -0.9f, 0.0f, 0.0f });
    AddComponent(std::move(left));

    auto right = std::make_unique<RacketComponent>(this, 0.1f, 0.4f);
    mRightRacket = right.get();
    mRightRacket->GetTransform()->SetPosition({ 0.8f, 0.0f, 0.0f });
    AddComponent(std::move(right));

    auto ball = std::make_unique<BallComponent>(this, 0.05f);
    mBall = ball.get();
    AddComponent(std::move(ball));
}

void PongGame::OnUpdate(float)
{
    HandleInput();
    HandleCollisions();
}

void PongGame::HandleInput()
{
    auto input = GetInput();

    if (input->IsKeyDown(static_cast<int>(Keys::W)) &&
        !mLeftRacket->GetCollision()->Intersects(mTopWall))
    {
        mLeftRacket->GetTransform()->Translate({ 0.0f, 0.01f, 0.0f });
    }
    if (input->IsKeyDown(static_cast<int>(Keys::S)) &&
        !mLeftRacket->GetCollision()->Intersects(mBottomWall))
    {
        mLeftRacket->GetTransform()->Translate({ 0.0f, -0.01f, 0.0f });
    }

    if (input->IsKeyDown(static_cast<int>(Keys::Up)) &&
        !mRightRacket->GetCollision()->Intersects(mTopWall))
    {
        mRightRacket->GetTransform()->Translate({ 0.0f, 0.01f, 0.0f });
    }
    if (input->IsKeyDown(static_cast<int>(Keys::Down)) &&
        !mRightRacket->GetCollision()->Intersects(mBottomWall))
    {
        mRightRacket->GetTransform()->Translate({ 0.0f, -0.01f, 0.0f });
    }
}

void PongGame::HandleCollisions()
{
    if (mBall->GetCollision()->Intersects(mTopWall) ||
        mBall->GetCollision()->Intersects(mBottomWall))
    {
        auto v = mBall->GetVelocity();
        v.y = -v.y;
        mBall->SetVelocity(v);
    }

    if (mBall->GetCollision()->Intersects(mLeftWall))
    {
        mRightScore++;
        PrintScore();
        ResetBall(true);
    }
    if (mBall->GetCollision()->Intersects(mRightWall))
    {
        mLeftScore++;
        PrintScore();
        ResetBall(false);
    }

    if (mBall->GetCollision()->Intersects(
        mLeftRacket->GetCollision()->GetWorldBounds()))
    {
        HitBall(true);
    }
    if (mBall->GetCollision()->Intersects(
        mRightRacket->GetCollision()->GetWorldBounds()))
    {
        HitBall(false);
    }
}

void PongGame::HitBall(bool isLeftRacket)
{
    float direction = isLeftRacket ? 1.0f : -1.0f;
    auto velocity = mBall->GetVelocity();

    velocity.x = direction * fabs(velocity.x) * mBallSpeedMultiplier;
    velocity.y *= mBallSpeedMultiplier;

    mBall->SetVelocity(velocity);
}

void PongGame::ResetBall(bool leftLost)
{
    float direction = leftLost ? 1.0f : -1.0f;
    mBall->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
    mBall->SetVelocity({ 0.5f * direction, 0.3f * direction, 0.0f });
}

void PongGame::PrintScore()
{
    std::cout << "Score: Left " << mLeftScore << " - Right " << mRightScore << std::endl;
}
