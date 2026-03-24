#include "CelestialMotionComponent.h"
#include "Engine/Camera/FpsCamera.h"
#include "Engine/Camera/PerspectiveCameraBase.h"
#include "Graphics/BoxComponent.h"
#include "Graphics/SphericalComponent.h"
#include "PlanetariumGame.h"
#include "Window\Keys.h"

PlanetariumGame::PlanetariumGame(int width, int height)
    : GameApp(width, height)
{
}

void PlanetariumGame::OnCreateGame()
{
    auto sunRadius = 1.5f;
    auto sunMesh = std::make_unique<SphericalComponent>(this, sunRadius);
    sunMesh->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	SphericalComponent* sunMeshPtr = sunMesh.get();

    auto sunMotion = std::make_unique<CelestialMotionComponent>(
        this, sunMesh.get(), nullptr, 0.f, 0.f, 0.2f);
    AddComponent(std::move(sunMesh));
    AddComponent(std::move(sunMotion));

    auto mercuryMesh = std::make_unique<SphericalComponent>(this, 0.2f);
    mercuryMesh->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });

    auto mercuryMotion = std::make_unique<CelestialMotionComponent>(
        this, mercuryMesh.get(), sunMeshPtr, 2.0f, 1.2f, 0.5f);
    AddComponent(std::move(mercuryMesh));
    AddComponent(std::move(mercuryMotion));

    auto venusMesh = std::make_unique<BoxComponent>(this, 0.3f, 0.3f, 0.3f);
    venusMesh->SetColor({ 1.0f, 0.8f, 0.0f, 1.0f });

    auto venusMotion = std::make_unique<CelestialMotionComponent>(
        this, venusMesh.get(), sunMeshPtr, 3.5f, 0.8f, 0.4f);
    AddComponent(std::move(venusMesh));
    AddComponent(std::move(venusMotion));

    auto earthRadius = 0.5f;
    auto earthMesh = std::make_unique<SphericalComponent>(this, earthRadius);
    earthMesh->SetColor({ 0.0f, 0.1f, 0.9f, 1.0f });
	SphericalComponent* earthMeshPtr = earthMesh.get();

    auto earthMotion = std::make_unique<CelestialMotionComponent>(
        this, earthMesh.get(), sunMeshPtr, 5.0f, 0.5f, 0.5f);
    AddComponent(std::move(earthMesh));
    AddComponent(std::move(earthMotion));

    auto moonMesh = std::make_unique<SphericalComponent>(this, 0.2f);
    moonMesh->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });

    auto moonMotion = std::make_unique<CelestialMotionComponent>(
        this, moonMesh.get(), earthMeshPtr, 1.5f, 2.0f, 0.6f);
    AddComponent(std::move(moonMesh));
    AddComponent(std::move(moonMotion));

    auto marsMesh = std::make_unique<BoxComponent>(this, 0.3f, 0.3f, 0.3f);
    marsMesh->SetColor({ 0.9f, 0.3f, 0.1f, 1.0f });
	BoxComponent* marsMeshPtr = marsMesh.get();

    auto marsMotion = std::make_unique<CelestialMotionComponent>(
        this, marsMesh.get(), sunMeshPtr, 8.0f, 0.3f, 0.3f);
    AddComponent(std::move(marsMesh));
    AddComponent(std::move(marsMotion));

    auto phobosMesh = std::make_unique<SphericalComponent>(this, 0.1f);
    phobosMesh->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

    auto phobosMotion = std::make_unique<CelestialMotionComponent>(
        this, phobosMesh.get(), marsMeshPtr, 0.6f, 1.5f, 0.6f);
    AddComponent(std::move(phobosMesh));
    AddComponent(std::move(phobosMotion));

    auto deimosMesh = std::make_unique<SphericalComponent>(this, 0.08f);
    deimosMesh->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });

    auto deimosMotion = std::make_unique<CelestialMotionComponent>(
        this, deimosMesh.get(), marsMeshPtr, 1.0f, 1.2f, 0.5f);
    AddComponent(std::move(deimosMesh));
    AddComponent(std::move(deimosMotion));

    auto jupiterMesh = std::make_unique<SphericalComponent>(this, 1.0f);
    jupiterMesh->SetColor({ 0.9f, 0.6f, 0.3f, 1.0f });

    auto jupiterMotion = std::make_unique<CelestialMotionComponent>(
        this, jupiterMesh.get(), sunMeshPtr, 11.0f, 0.2f, 0.2f);
    AddComponent(std::move(jupiterMesh));
    AddComponent(std::move(jupiterMotion));

    auto boxPlanet = std::make_unique<BoxComponent>(this, 0.4f, 0.4f, 0.4f);
    boxPlanet->SetColor({ 0.2f, 1.0f, 0.2f, 1.0f });

    auto boxMotion = std::make_unique<CelestialMotionComponent>(
        this, boxPlanet.get(), sunMeshPtr, 12.0f, 0.05f, 0.3f);
    AddComponent(std::move(boxPlanet));
    AddComponent(std::move(boxMotion));

    float fovYRad = DirectX::XMConvertToRadians(mSunCamera->GetFov());
    float distance = sunRadius / std::tan(fovYRad * 0.5f);

    auto sunCameraMotion = std::make_unique<CelestialMotionComponent>(
        this, mSunCamera, sunMeshPtr, distance, 0.f, 0.f);
	mSunCamera->SetTarget(sunMeshPtr);
    AddComponent(std::move(sunCameraMotion));

    fovYRad = DirectX::XMConvertToRadians(mEarthCamera->GetFov());
    distance = earthRadius / std::tan(fovYRad * 0.5f);

    auto earthCameraMotion = std::make_unique<CelestialMotionComponent>(
        this, mEarthCamera, earthMeshPtr, distance, 0.f, 0.f);
    mEarthCamera->SetTarget(earthMeshPtr);
    AddComponent(std::move(earthCameraMotion));
}

void PlanetariumGame::CreateCamera()
{
    float aspect = static_cast<float>(mClientWidth) / mClientHeight;

    auto fpsCam = std::make_unique<FpsCamera>(this);
    fpsCam->SetAspect(aspect);
    mFpsCamera = fpsCam.get();
    AddComponent(std::move(fpsCam));

    auto sunCamera = std::make_unique<PerspectiveCameraBase>(this);
    sunCamera->SetAspect(aspect);
	mSunCamera = sunCamera.get();
    AddComponent(std::move(sunCamera));

    auto earthCamera = std::make_unique<PerspectiveCameraBase>(this);
    earthCamera->SetAspect(aspect);
    mEarthCamera = earthCamera.get();
    AddComponent(std::move(earthCamera));

    mCamera = mSunCamera;
}

void PlanetariumGame::OnUpdate(float delta)
{
    auto input = GetInput();
    auto transform = mCamera->GetTransform();

    Vector3 forward = mCamera->GetForward();
    Vector3 right = mCamera->GetRight();

    if (input->IsKeyDown(static_cast<int>(Keys::W)))
        transform->Translate(forward * moveSpeed * delta);

    if (input->IsKeyDown(static_cast<int>(Keys::S)))
        transform->Translate(forward * moveSpeed * delta * -1);

    if (input->IsKeyDown(static_cast<int>(Keys::D)))
        transform->Translate(right * moveSpeed * delta);

    if (input->IsKeyDown(static_cast<int>(Keys::A)))
        transform->Translate(right * moveSpeed * delta * -1);

    if (mInput->IsKeyPressed(static_cast<int>(Keys::C)))
    {
        mCamera = mFpsCamera;
    }
    if (mInput->IsKeyPressed(static_cast<int>(Keys::D1)))
    {
        mCamera = mSunCamera;
    }
    if (mInput->IsKeyPressed(static_cast<int>(Keys::D2)))
    {
        mCamera = mEarthCamera;
    }
}
