#include <SimpleMath.h>

#include "Engine/Camera/FpsCamera.h"
#include "Engine/Graphics/Mesh/BoxComponent.h"
#include "Engine/Graphics/Mesh/SphericalComponent.h"
#include "Engine/Graphics/RenderableComponent.h"
#include "Engine/Window/Keys.h"
#include "KatamariDamacyGame.h"

using DirectX::SimpleMath::Quaternion;

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

    SpawnRandomObjects(6);
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

float KatamariDamacyGame::RandomRange(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mRandom);
}

void KatamariDamacyGame::SpawnRandomObjects(int count)
{
    for (int i = 0; i < count; ++i)
    {
        Vector3 pos{
            RandomRange(-10.f, 10.f),
            0.f,
            RandomRange(0.f, 10.f)
        };

        if (i % 2 == 0)
            CreateMike(pos);
        else
            CreateKnuckles(pos);
    }
}

void KatamariDamacyGame::CreateMike(const Vector3& position)
{
    auto visual = std::make_unique<RenderableComponent>(
        this,
        "./Content/MikeWazovski/Mike.fbx",
        "./Content/MikeWazovski/Mike_Material_BaseColor.dds");

    visual->GetTransform()->SetScale({ 0.5f, 0.5f, 0.5f });

    visual->GetTransform()->Rotate({
        DirectX::XMConvertToRadians(-90.f),
        DirectX::XMConvertToRadians(90.f),
        0.f
    });

    auto collision = std::make_unique<SphericalComponent>(this, 0.5f);
    collision->GetTransform()->Translate({ 0.f, 0.4f, 0.1f }); // + влево, вверх, вперед
    collision->SetVisible(false);

    auto mesh = std::make_unique<CollisionMesh>(
        this,
        visual.get(),
        collision.get());

    mesh->GetTransform()->SetPosition(position);

    mStickObjects.push_back(std::move(mesh));

    AddComponent(std::move(visual));
    AddComponent(std::move(collision));
}

void KatamariDamacyGame::CreateKnuckles(const Vector3& position)
{
    auto visual = std::make_unique<RenderableComponent>(
        this,
        "./Content/UgandanKnuckles/knucklesModel.OBJ",
        "./Content/UgandanKnuckles/knucklesDIFF3.dds");

    visual->GetTransform()->SetScale({ 0.06f, 0.06f, 0.06f });
    visual->GetTransform()->Translate({ 0.f, -0.3f, 0.f });

    auto collision = std::make_unique<BoxComponent>(
        this,
        1.2f, 1.2f, 0.5f);

    collision->GetTransform()->Translate({ 0.f, 0.2f, 0.f });
    collision->SetVisible(false);

    auto mesh = std::make_unique<CollisionMesh>(
        this,
        visual.get(),
        collision.get());

    mesh->GetTransform()->SetPosition(position);

    mStickObjects.push_back(std::move(mesh));

    AddComponent(std::move(visual));
    AddComponent(std::move(collision));
}

void KatamariDamacyGame::UpdateCamera()
{
    auto input = GetInput();
    mCameraYaw -= mCameraRotateSpeed * input->GetMouseDeltaX();

    Vector3 center = mBall->GetTransform()->GetPosition();

    Vector3 offset;
    offset.x = sinf(mCameraYaw) * mCameraDistance * -1;
    offset.y = mCameraHeight;
    offset.z = cosf(mCameraYaw) * mCameraDistance * -1;

    Vector3 cameraPos = center + offset;

    mCamera->GetTransform()->SetPosition(cameraPos);
}

void KatamariDamacyGame::UpdateBall(float delta)
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

        moveDir *= -1;
        Vector3 rotationAxis = moveDir.Cross(Vector3::Up);
        rotationAxis.Normalize();

        float angle = distance / mBallRadius;

        ballTransform->Rotate(rotationAxis * angle);
    }
}

void KatamariDamacyGame::CheckCollisions()
{
    for (auto& object : mStickObjects)
    {
        if (mBall->GetCollision()->Intersects(*object->GetCollision()))
        {
            if (object->GetTransform()->GetParent() != mBall->GetTransform())
            {
                auto objectTransform = object->GetTransform();
                auto ballTransform = mBall->GetTransform();

                Vector3 objectWorldPos = objectTransform->GetPosition();
                Quaternion objectWorldRot = objectTransform->GetRotationQuaternion();

                // Преобразуем позицию в локальные координаты шара
                Matrix ballWorldMatrix = ballTransform->GetWorldMatrix();
                Matrix ballWorldInverse = ballWorldMatrix.Invert();
                Vector3 localPos = Vector3::Transform(objectWorldPos, ballWorldInverse);

                // Преобразуем вращение в локальные координаты шара
                Quaternion ballWorldRot = ballTransform->GetRotationQuaternion();
                ballWorldRot.Conjugate();
                Quaternion localRot = objectWorldRot * ballWorldRot;

                objectTransform->SetParent(ballTransform);
                objectTransform->SetPosition(localPos);
                objectTransform->SetRotationQuaternion(localRot);
            }
        }
    }
}

void KatamariDamacyGame::OnUpdate(float delta)
{
    UpdateBall(delta);
    UpdateCamera();
    CheckCollisions();
}
