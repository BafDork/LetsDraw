#include "CelestialBodyComponent.h"
#include "Graphics\CameraComponent.h"
#include "PlanetariumGame.h"
#include "Window\Keys.h"

PlanetariumGame::PlanetariumGame(int width, int height)
    : GameApp(width, height)
{
}

void PlanetariumGame::OnCreateGame()
{
    auto sun = std::make_unique<CelestialBodyComponent>(this, 1.5f, nullptr, 0.f, 0.f);
    sun->GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
    sun->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
    CelestialBodyComponent* pSun = sun.get();
    AddComponent(std::move(sun));

    auto mercury = std::make_unique<CelestialBodyComponent>(this, 0.2f, pSun, 2.0f, 1.2f);
    mercury->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });
    AddComponent(std::move(mercury));

    auto venus = std::make_unique<CelestialBodyComponent>(this, 0.3f, pSun, 3.5f, 0.8f);
    venus->SetColor({ 1.0f, 0.8f, 0.0f, 1.0f });
    AddComponent(std::move(venus));

    auto earth = std::make_unique<CelestialBodyComponent>(this, 0.5f, pSun, 5.0f, 0.5f);
    earth->SetColor({ 0.0f, 0.1f, 0.9f, 1.0f });
    CelestialBodyComponent* pEarth = earth.get();
    AddComponent(std::move(earth));

    auto moon = std::make_unique<CelestialBodyComponent>(this, 0.2f, pEarth, 1.5f, 2.0f);
    moon->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });
    AddComponent(std::move(moon));

    auto mars = std::make_unique<CelestialBodyComponent>(this, 0.3f, pSun, 8.0f, 0.3f);
    mars->SetColor({ 0.9f, 0.3f, 0.1f, 1.0f });
    CelestialBodyComponent* pMars = mars.get();
    AddComponent(std::move(mars));

    auto phobos = std::make_unique<CelestialBodyComponent>(this, 0.1f, pMars, 0.6f, 1.5f);
    phobos->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });
    AddComponent(std::move(phobos));

    auto deimos = std::make_unique<CelestialBodyComponent>(this, 0.08f, pMars, 1.0f, 1.2f);
    deimos->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });
    AddComponent(std::move(deimos));

    auto jupiter = std::make_unique<CelestialBodyComponent>(this, 1.0f, pSun, 11.0f, 0.2f);
    jupiter->SetColor({ 0.9f, 0.6f, 0.3f, 1.0f });
    AddComponent(std::move(jupiter));

    auto saturn = std::make_unique<CelestialBodyComponent>(this, 0.9f, pSun, 14.0f, 0.15f);
    saturn->SetColor({ 0.9f, 0.8f, 0.5f, 1.0f });
    AddComponent(std::move(saturn));

    auto uranus = std::make_unique<CelestialBodyComponent>(this, 0.7f, pSun, 17.0f, 0.1f);
    uranus->SetColor({ 0.5f, 0.8f, 1.0f, 1.0f });
    AddComponent(std::move(uranus));

    auto neptune = std::make_unique<CelestialBodyComponent>(this, 0.7f, pSun, 20.0f, 0.08f);
    neptune->SetColor({ 0.3f, 0.3f, 1.0f, 1.0f });
    AddComponent(std::move(neptune));
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
