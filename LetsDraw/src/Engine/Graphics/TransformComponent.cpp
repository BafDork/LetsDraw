#include "TransformComponent.h"

Matrix TransformComponent::GetWorldMatrix() const
{
    Matrix ScalingMatrix = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
    Matrix RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
    Matrix TranslationMatrix = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

    Matrix local = ScalingMatrix * RotationMatrix * TranslationMatrix;

    if (mParent)
        return local * mParent->GetWorldMatrix();

    return local;
}
