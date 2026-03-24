#pragma once

#include <SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class TransformComponent
{
public:
    TransformComponent() = default;

    void SetPosition(const Vector3& position) { mPosition = position; }
    void SetRotation(const Vector3& rotation) { mRotation = rotation; }
    void SetScale(const Vector3& scale) { mScale = scale; }

    void Translate(const Vector3& delta) { mPosition += delta; }
    void Rotate(const Vector3& delta) { mRotation += delta; }

    Matrix GetWorldMatrix() const;

    Vector3 GetPosition() const { return mPosition; }
    Vector3 GetRotation() const { return mRotation; }
    Vector3 GetScale() const { return mScale; }

private:
    Vector3 mPosition{ 0.0f, 0.0f, 0.0f };
    Vector3 mRotation{ 0.0f, 0.0f, 0.0f };  // pitch yaw roll
    Vector3 mScale{ 1.0f, 1.0f, 1.0f };
};
