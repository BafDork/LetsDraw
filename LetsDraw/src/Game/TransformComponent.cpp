#include "TransformComponent.h"

void TransformComponent::SetPosition(const XMFLOAT3& position)
{
    this->position = position;
}

void TransformComponent::Translate(const XMFLOAT3& delta)
{
    position.x += delta.x;
    position.y += delta.y;
    position.z += delta.z;
}

void TransformComponent::SetScale(const XMFLOAT3& scale)
{
    this->scale = scale;
}

void TransformComponent::SetRotation(const XMFLOAT3& rotation)
{
    this->rotation = rotation;
}

void TransformComponent::Rotate(const XMFLOAT3& delta)
{
    rotation.x += delta.x;
    rotation.y += delta.y;
    rotation.z += delta.z;
}

XMMATRIX TransformComponent::GetWorldMatrix() const
{
    XMMATRIX ScalingMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMMATRIX TranslationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    return ScalingMatrix * RotationMatrix * TranslationMatrix;
}
