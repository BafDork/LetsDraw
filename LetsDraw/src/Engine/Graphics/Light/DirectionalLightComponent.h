#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <wrl.h>

#include "BaseLightComponent.h"

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class DirectionalLightComponent : public BaseLightComponent
{
public:
    DirectionalLightComponent(GameApp* game);

    void Initialize() override;
    void Update(float deltaTime) override;

    void SetDirection(const Vector3& dir);
    Vector3 GetDirection() const;

    ID3D11ShaderResourceView* GetShadowSRV() const { return mShadowSRV.Get(); }
    ID3D11DepthStencilView* GetShadowDSV() const { return mShadowDSV.Get(); }
    const Matrix& GetLightViewProj() const { return mLightViewProj; }

private:
    void CreateShadowMap();

private:
    Vector3 mDirection = { 0.3f, -1.0f, 0.2f };

    ComPtr<ID3D11Texture2D> mShadowTexture;
    ComPtr<ID3D11DepthStencilView> mShadowDSV;
    ComPtr<ID3D11ShaderResourceView> mShadowSRV;

    Matrix mLightViewProj;
};
