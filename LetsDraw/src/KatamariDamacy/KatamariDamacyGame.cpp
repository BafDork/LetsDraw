#include "Engine/Camera/FpsCamera.h"
#include "Engine/Graphics/Mesh/SphericalComponent.h"
#include "Engine/Graphics/RenderableComponent.h"
#include "Engine/Window/Keys.h"
#include "KatamariDamacyGame.h"

KatamariDamacyGame::KatamariDamacyGame(int width, int height)
    : GameApp(width, height)
{
}

void KatamariDamacyGame::OnCreateGame()
{
    auto ballMesh = std::make_unique<SphericalComponent>(this, mBallRadius);
    ballMesh->SetColor({ 0.5f, 0.5f, 0.5f, 1.0f });
    mBall = ballMesh.get();
    AddComponent(std::move(ballMesh));

    auto mikeMesh = std::make_unique<RenderableComponent>(
        this,
        "./Content/MikeWazovski/Mike.fbx",
        "./Content/MikeWazovski/Mike_Material_BaseColor.dds");

    mikeMesh->GetTransform()->Translate({0.0f, 0.f, 5.f});
    mikeMesh->GetTransform()->Rotate({
         DirectX::XMConvertToRadians(-90.f),
         DirectX::XMConvertToRadians(90.f),
         DirectX::XMConvertToRadians(0.f)
    });

    AddComponent(std::move(mikeMesh));
}

void KatamariDamacyGame::CreateCamera()
{
    float aspect = static_cast<float>(mClientWidth) / mClientHeight;

    auto fpsCam = std::make_unique<FpsCamera>(this);
    fpsCam->SetAspect(aspect);
    mFpsCamera = fpsCam.get();
    AddComponent(std::move(fpsCam));

    mCamera = mFpsCamera;
}

void KatamariDamacyGame::OnUpdate(float delta)
{
    auto input = GetInput();
	auto ballTransform = mBall->GetTransform();

    Vector3 forward = mCamera->GetForward();
    Vector3 right = mCamera->GetRight();

    forward.y = 0.f;
    right.y = 0.f;

    forward.Normalize();
    right.Normalize();

    Vector3 moveDir = Vector3::Zero;

    if (input->IsKeyDown(static_cast<int>(Keys::W)))
        moveDir += forward;

    if (input->IsKeyDown(static_cast<int>(Keys::S)))
        moveDir -= forward;

    if (input->IsKeyDown(static_cast<int>(Keys::D)))
        moveDir += right;

    if (input->IsKeyDown(static_cast<int>(Keys::A)))
        moveDir -= right;

    if (moveDir != Vector3::Zero)
    {
        moveDir.Normalize();

        float distance = moveSpeed * delta;
        ballTransform->Translate(moveDir * distance);

        Vector3 rotationAxis = moveDir.Cross(Vector3::Up);
        rotationAxis.Normalize();

        float angle = distance / mBallRadius;

        ballTransform->Rotate(rotationAxis * angle);
    }

    mCamera->GetTransform()->SetPosition({
        ballTransform->GetPosition().x,
        ballTransform->GetPosition().y + 2.f,
        ballTransform->GetPosition().z - 2.f,
    });
}
