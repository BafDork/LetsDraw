#pragma once

#include <random>
#include <SimpleMath.h>

#include "Engine/GameApp.h"
#include "Engine/Graphics/Mesh/CollisionMesh.h"

using DirectX::SimpleMath::Vector3;

class SphericalComponent;
class WaveSurfaceComponent;

class KatamariDamacyGame : public GameApp
{
public:
    KatamariDamacyGame(int width, int height);

protected:
    void CreateCamera() override;
    void OnCreateGame() override;
    void OnUpdate(float delta) override;

private:
    void CheckCollisions();
    void UpdateBall(float delta);
    void UpdateCamera();

    float RandomRange(float min, float max);
    void SpawnRandomObjects(int count);
    void CreateMike(const Vector3& position);
    void CreateKnuckles(const Vector3& position);
    void SnapToGround(TransformComponent* transform, float heightOffset);
    void SpawnLightVortex(int count);

private:
    CameraBase* mFpsCamera = nullptr;
	SphericalComponent* mBall = nullptr;
    WaveSurfaceComponent* mFloor = nullptr;

    std::mt19937 mRandom{ std::random_device{}() };

    std::vector<std::unique_ptr<CollisionMesh>> mStickObjects;

    float moveSpeed = 3.0f;
	float mBallRadius = 0.5f;

    float mCameraYaw = 0.f;
    float mCameraDistance = 2.5f;
    float mCameraHeight = 1.5f;
    float mCameraRotateSpeed = 0.001f;
};
