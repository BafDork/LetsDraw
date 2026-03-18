#include "Graphics\CameraComponent.h"
#include "PlanetariumGame.h"
#include "Window\Keys.h"
#include "PongGame\RacketComponent.h"

PlanetariumGame::PlanetariumGame(int width, int height)
    : GameApp(width, height)
{
}

void PlanetariumGame::OnCreateGame()
{
    auto left = std::make_unique<RacketComponent>(this, 0.1f, 0.4f);
    left->GetTransform()->SetPosition({ -0.9f, 0.0f, 0.0f });
    AddComponent(std::move(left));
}

void PlanetariumGame::OnUpdate(float delta)
{
    auto input = GetInput();

    if (input->IsKeyDown(static_cast<int>(Keys::W)))
    {
        mCamera->Move(mCamera->GetForward() * moveSpeed * delta);
    }
    if (input->IsKeyDown(static_cast<int>(Keys::S)))
    {
        mCamera->Move(mCamera->GetForward() * moveSpeed * delta * -1);
    }
    if (input->IsKeyDown(static_cast<int>(Keys::D)))
    {
        mCamera->Move(mCamera->GetRight() * moveSpeed * delta);
    }
    if (input->IsKeyDown(static_cast<int>(Keys::A)))
    {
        mCamera->Move(mCamera->GetRight() * moveSpeed * delta * -1);
    }
}
