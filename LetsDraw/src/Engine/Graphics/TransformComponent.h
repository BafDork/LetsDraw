#pragma once

#include <SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

class TransformComponent
{
public:
    TransformComponent() = default;

    void SetPosition(const Vector3& position) { mPosition = position; };
    Vector3 GetPosition() const { return mPosition; };
    void Translate(const Vector3& delta) { mPosition += delta; };

    void SetRotation(const Vector3& eulerRadians);
    void Rotate(const Vector3& deltaEuler);

    void SetRotationQuaternion(const Quaternion& quat) { mRotationQuat = quat; };
    Quaternion GetRotationQuaternion() const { return mRotationQuat; };

    Vector3 GetScale() const { return mScale; };
    void SetScale(const Vector3& scale) { mScale = scale; };

    TransformComponent* GetParent() const { return mParent; };
    void SetParent(TransformComponent* parent) { mParent = parent; };

    Matrix GetLocalMatrix() const;
    Matrix GetWorldMatrix() const;

    Vector3 GetLocalPositionRelativeToParent() const;
    void SetLocalPositionRelativeToParent(const Vector3& localPos);

private:
    TransformComponent* mParent = nullptr;

    Vector3 mPosition{ 0,0,0 };
    Vector3 mScale{ 1,1,1 };
    Quaternion mRotationQuat{ Quaternion::Identity };
};
