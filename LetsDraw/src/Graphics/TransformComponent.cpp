#include "TransformComponent.h"

void TransformComponent::SetPosition(const XMFLOAT3& position)
{
    mPosition = position;
}

void TransformComponent::Translate(const XMFLOAT3& delta)
{
    mPosition.x += delta.x;
    mPosition.y += delta.y;
    mPosition.z += delta.z;
}

void TransformComponent::SetScale(const XMFLOAT3& scale)
{
    mScale = scale;
}

void TransformComponent::SetRotation(const XMFLOAT3& rotation)
{
    mRotation = rotation;
}

void TransformComponent::Rotate(const XMFLOAT3& delta)
{
    mRotation.x += delta.x;
    mRotation.y += delta.y;
    mRotation.z += delta.z;
}

XMMATRIX TransformComponent::GetWorldMatrix() const
{
    XMMATRIX ScalingMatrix = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
    XMMATRIX RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
    XMMATRIX TranslationMatrix = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

    return ScalingMatrix * RotationMatrix * TranslationMatrix;
}
