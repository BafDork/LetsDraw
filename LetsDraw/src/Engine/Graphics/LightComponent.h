#pragma once

#include "Engine/GameComponent.h"
#include "ITransformProvider.h"
#include "TransformComponent.h"

#include <d3d11.h>
#include <memory>
#include <SimpleMath.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

using DirectX::SimpleMath::Vector3;

class GameApp;

class LightComponent : public GameComponent, public ITransformProvider
{
public:
    LightComponent(GameApp* game);

    void Initialize() override;
    void Update(float deltaTime) override;

    TransformComponent* GetTransform() { return mTransform.get(); }

    void SetDirection(const Vector3& dir);
    Vector3 GetDirection() const;

    void SetIntensity(float intensity) { mIntensity = intensity; }
    float GetIntensity() const { return mIntensity; }

    ID3D11ShaderResourceView* GetShadowSRV() const { return mShadowSRV.Get(); }
    ID3D11DepthStencilView* GetShadowDSV() const { return mShadowDSV.Get(); }
    const Matrix& GetLightViewProj() const { return mLightViewProj; }

private:
    void CreateShadowMap();

private:
    ComPtr<ID3D11Texture2D> mShadowTexture;
    ComPtr<ID3D11DepthStencilView> mShadowDSV;
    ComPtr<ID3D11ShaderResourceView> mShadowSRV;

    Matrix mLightViewProj;
    std::unique_ptr<TransformComponent> mTransform;

    Vector3 mDirection = { 0.3f, -1.0f, 0.2f };
    float mIntensity = 1.0f;
};