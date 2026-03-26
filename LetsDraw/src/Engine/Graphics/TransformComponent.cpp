#include "TransformComponent.h"

void TransformComponent::SetRotation(const Vector3& eulerRadians)
{
    mRotationQuat =
        Quaternion::CreateFromYawPitchRoll(
            eulerRadians.y,
            eulerRadians.x,
            eulerRadians.z);
}

void TransformComponent::Rotate(const Vector3& deltaEuler)
{
    Quaternion deltaQuat =
        Quaternion::CreateFromYawPitchRoll(
            deltaEuler.y,
            deltaEuler.x,
            deltaEuler.z);

    mRotationQuat *= deltaQuat;
}

Matrix TransformComponent::GetLocalMatrix() const
{
    return
        Matrix::CreateScale(mScale) *
        Matrix::CreateFromQuaternion(mRotationQuat) *
        Matrix::CreateTranslation(mPosition);
}

Matrix TransformComponent::GetWorldMatrix() const
{
    Matrix local = GetLocalMatrix();

    if (mParent)
        return local * mParent->GetWorldMatrix();

    return local;
}

Vector3 TransformComponent::GetLocalPositionRelativeToParent() const
{
    if (!mParent)
        return mPosition;

    Matrix parentWorldInv = mParent->GetWorldMatrix().Invert();
    return Vector3::Transform(mPosition, parentWorldInv);
}

void TransformComponent::SetLocalPositionRelativeToParent(const Vector3& localPos)
{
    if (!mParent)
        mPosition = localPos;
    else
    {
        Matrix parentWorld = mParent->GetWorldMatrix();
        mPosition = Vector3::Transform(localPos, parentWorld);
    }
}
