#include "CelestialMotionComponent.h"
#include "Graphics/BoxComponent.h"
#include "Graphics/SphericalComponent.h"
#include "Graphics\CameraComponent.h"
#include "PlanetariumGame.h"
#include "Window\Keys.h"

PlanetariumGame::PlanetariumGame(int width, int height)
    : GameApp(width, height)
{
}

void PlanetariumGame::OnCreateGame()
{
    auto sunMesh = std::make_unique<SphericalComponent>(this, 1.5f);
    sunMesh->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
    TransformComponent* sunTransform = sunMesh->GetTransform();

    auto sunMotion = std::make_unique<CelestialMotionComponent>(
        this, sunTransform, nullptr, 0.f, 0.f, 0.2f);
    AddComponent(std::move(sunMesh));
    AddComponent(std::move(sunMotion));

    auto mercuryMesh = std::make_unique<SphericalComponent>(this, 0.2f);
    mercuryMesh->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });

    auto mercuryMotion = std::make_unique<CelestialMotionComponent>(
        this, mercuryMesh->GetTransform(), sunTransform, 2.0f, 1.2f, 0.5f);
    AddComponent(std::move(mercuryMesh));
    AddComponent(std::move(mercuryMotion));

    auto venusMesh = std::make_unique<BoxComponent>(this, 0.3f, 0.3f, 0.3f);
    venusMesh->SetColor({ 1.0f, 0.8f, 0.0f, 1.0f });

    auto venusMotion = std::make_unique<CelestialMotionComponent>(
        this, venusMesh->GetTransform(), sunTransform, 3.5f, 0.8f, 0.4f);
    AddComponent(std::move(venusMesh));
    AddComponent(std::move(venusMotion));

    auto earthMesh = std::make_unique<SphericalComponent>(this, 0.5f);
    earthMesh->SetColor({ 0.0f, 0.1f, 0.9f, 1.0f });
    TransformComponent* earthTransform = earthMesh->GetTransform();

    auto earthMotion = std::make_unique<CelestialMotionComponent>(
        this, earthTransform, sunTransform, 5.0f, 0.5f, 0.5f);
    AddComponent(std::move(earthMesh));
    AddComponent(std::move(earthMotion));

    auto moonMesh = std::make_unique<SphericalComponent>(this, 0.2f);
    moonMesh->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });

    auto moonMotion = std::make_unique<CelestialMotionComponent>(
        this, moonMesh->GetTransform(), earthTransform, 1.5f, 2.0f, 0.6f);
    AddComponent(std::move(moonMesh));
    AddComponent(std::move(moonMotion));

    auto marsMesh = std::make_unique<BoxComponent>(this, 0.3f, 0.3f, 0.3f);
    marsMesh->SetColor({ 0.9f, 0.3f, 0.1f, 1.0f });
    TransformComponent* marsTransform = marsMesh->GetTransform();

    auto marsMotion = std::make_unique<CelestialMotionComponent>(
        this, marsTransform, sunTransform, 8.0f, 0.3f, 0.3f);
    AddComponent(std::move(marsMesh));
    AddComponent(std::move(marsMotion));

    auto phobosMesh = std::make_unique<SphericalComponent>(this, 0.1f);
    phobosMesh->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

    auto phobosMotion = std::make_unique<CelestialMotionComponent>(
        this, phobosMesh->GetTransform(), marsTransform, 0.6f, 1.5f, 0.6f);
    AddComponent(std::move(phobosMesh));
    AddComponent(std::move(phobosMotion));

    auto deimosMesh = std::make_unique<SphericalComponent>(this, 0.08f);
    deimosMesh->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });

    auto deimosMotion = std::make_unique<CelestialMotionComponent>(
        this, deimosMesh->GetTransform(), marsTransform, 1.0f, 1.2f, 0.5f);
    AddComponent(std::move(deimosMesh));
    AddComponent(std::move(deimosMotion));

    auto jupiterMesh = std::make_unique<SphericalComponent>(this, 1.0f);
    jupiterMesh->SetColor({ 0.9f, 0.6f, 0.3f, 1.0f });

    auto jupiterMotion = std::make_unique<CelestialMotionComponent>(
        this, jupiterMesh->GetTransform(), sunTransform, 11.0f, 0.2f, 0.2f);
    AddComponent(std::move(jupiterMesh));
    AddComponent(std::move(jupiterMotion));

    auto boxPlanet = std::make_unique<BoxComponent>(this, 0.4f, 0.4f, 0.4f);
    boxPlanet->SetColor({ 0.2f, 1.0f, 0.2f, 1.0f });

    auto boxMotion = std::make_unique<CelestialMotionComponent>(
        this, boxPlanet->GetTransform(), sunTransform, 22.0f, 0.05f, 0.3f);
    AddComponent(std::move(boxPlanet));
    AddComponent(std::move(boxMotion));
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
