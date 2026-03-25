#include "Engine/Camera/FpsCamera.h"
#include "Engine/Graphics/RenderableComponent.h"
#include "Engine/Window/Keys.h"
#include "KatamariDamacyGame.h"

#include "Engine/Graphics/Mesh/SphericalComponent.h"

KatamariDamacyGame::KatamariDamacyGame(int width, int height)
    : GameApp(width, height)
{
}

void KatamariDamacyGame::OnCreateGame()
{
    auto mesh = std::make_unique<RenderableComponent>(
        this,
        "./Content/MikeWazovski/Mike.fbx",
        "./Content/MikeWazovski/Mike_Material_BaseColor.dds");

    mesh->GetTransform()->Rotate({
         DirectX::XMConvertToRadians(-90.f),
         DirectX::XMConvertToRadians(90.f),
         DirectX::XMConvertToRadians(0.f)
    });

    AddComponent(std::move(mesh));
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
}
