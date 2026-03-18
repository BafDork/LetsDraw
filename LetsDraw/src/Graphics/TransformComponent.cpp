#include "TransformComponent.h"

void TransformComponent::SetPosition(const Vector3& position)
{
    mPosition = position;
}

void TransformComponent::Translate(const Vector3& delta)
{
    mPosition.x += delta.x;
    mPosition.y += delta.y;
    mPosition.z += delta.z;
}

void TransformComponent::SetScale(const Vector3& scale)
{
    mScale = scale;
}

void TransformComponent::SetRotation(const Vector3& rotation)
{
    mRotation = rotation;
}

void TransformComponent::Rotate(const Vector3& delta)
{
    mRotation.x += delta.x;
    mRotation.y += delta.y;
    mRotation.z += delta.z;
}

Matrix TransformComponent::GetWorldMatrix() const
{
    Matrix ScalingMatrix = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
    Matrix RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
    Matrix TranslationMatrix = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

    return ScalingMatrix * RotationMatrix * TranslationMatrix;
}
