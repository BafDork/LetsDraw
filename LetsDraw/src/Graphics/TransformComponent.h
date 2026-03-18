#pragma once

#include <SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class TransformComponent
{
public:
    TransformComponent() = default;

    void SetPosition(const Vector3& position);
    void Translate(const Vector3& delta);

    void SetScale(const Vector3& scale);

    void SetRotation(const Vector3& rotation);
    void Rotate(const Vector3& delta);

    Matrix GetWorldMatrix() const;

    Vector3 GetPosition() const { return mPosition; }
    Vector3 GetRotation() const { return mRotation; }
    Vector3 GetScale() const { return mScale; }

private:
    Vector3 mPosition{ 0.0f, 0.0f, 0.0f };
    Vector3 mRotation{ 0.0f, 0.0f, 0.0f };
    Vector3 mScale{ 1.0f, 1.0f, 1.0f };
};
