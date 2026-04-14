#include "DirectionalLightComponent.h"
#include "Engine/GameApp.h"

DirectionalLightComponent::DirectionalLightComponent(GameApp* game)
    : BaseLightComponent(game, LightType::Directional)
{
    mDirection.Normalize();
}

void DirectionalLightComponent::Initialize()
{
    CreateShadowMap();
}

void DirectionalLightComponent::CreateShadowMap()
{
    auto device = mGameApp->GetDevice();

    D3D11_TEXTURE2D_DESC texDesc{};
    texDesc.Width = 2048;
    texDesc.Height = 2048;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    texDesc.SampleDesc.Count = 1;
    texDesc.BindFlags =
        D3D11_BIND_DEPTH_STENCIL |
        D3D11_BIND_SHADER_RESOURCE;

    device->CreateTexture2D(&texDesc, nullptr, &mShadowTexture);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    device->CreateDepthStencilView(
        mShadowTexture.Get(),
        &dsvDesc,
        &mShadowDSV);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    device->CreateShaderResourceView(
        mShadowTexture.Get(),
        &srvDesc,
        &mShadowSRV);
}

void DirectionalLightComponent::Update(float deltaTime)
{
    Vector3 lightPos = -mDirection * 50.0f;
    Vector3 target = { 0, 0, 0 };

    Matrix view = Matrix::CreateLookAt(lightPos, target, Vector3::Up);
    Matrix proj = Matrix::CreateOrthographic(40.f, 40.f, 1.f, 100.f);

    mLightViewProj = view * proj;
}

void DirectionalLightComponent::SetDirection(const Vector3& dir)
{
    mDirection = dir;
    mDirection.Normalize();
}

Vector3 DirectionalLightComponent::GetDirection() const
{
    return mDirection;
}
