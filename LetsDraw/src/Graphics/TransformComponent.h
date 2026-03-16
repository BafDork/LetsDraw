#pragma once

#include <DirectXMath.h>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

class TransformComponent
{
public:
    TransformComponent() = default;

    void SetPosition(const XMFLOAT3& position);
    void Translate(const XMFLOAT3& delta);

    void SetScale(const XMFLOAT3& scale);

    void SetRotation(const XMFLOAT3& rotation);
    void Rotate(const XMFLOAT3& delta);

    XMMATRIX GetWorldMatrix() const;

    XMFLOAT3 GetPosition() const { return mPosition; }
    XMFLOAT3 GetRotation() const { return mRotation; }
    XMFLOAT3 GetScale() const { return mScale; }

private:
    XMFLOAT3 mPosition{ 0.0f, 0.0f, 0.0f };
    XMFLOAT3 mRotation{ 0.0f, 0.0f, 0.0f };
    XMFLOAT3 mScale{ 1.0f, 1.0f, 1.0f };
};
