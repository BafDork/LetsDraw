#pragma once
#pragma comment(lib, "d3d11.lib")

#include "Engine\GameComponent.h"
#include "ITransformProvider.h"
#include "TransformComponent.h"
#include "Vertex.h"

#include <d3d11.h>
#include <memory>
#include <SimpleMath.h>
#include <string>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

class GameApp;
class CameraComponent;

class RenderableComponent : public GameComponent, public ITransformProvider
{
public:
    RenderableComponent(GameApp* gameApp);
    RenderableComponent(GameApp* gameApp, const std::string& modelFile, const std::string& textureFile);

    void Initialize() override;
    void Draw() override;

    TransformComponent* GetTransform() { return mTransform.get(); }
    void SetTransform(std::unique_ptr<TransformComponent> transform) { mTransform = std::move(transform); }

    void SetVisible(bool visible) { mVisible = visible; }
    bool IsVisible() const { return mVisible; }

protected:
    virtual void CreateShaders();
    virtual void CreateGeometry();
    virtual void CreateRasterizerState();
	virtual void CreateConstantBuffer();
    virtual void CreateMaterialBuffer();
    virtual void CreateLightBuffer();
    virtual void CreateSamplerState();

    virtual void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) {}

private:
    bool LoadTexture(const std::string& textureFile, ComPtr<ID3D11ShaderResourceView>& outSRV);

protected:
    std::unique_ptr<TransformComponent> mTransform;

private:
    std::vector<Vertex> mPoints;
    std::vector<uint32_t> mIndices;

    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mContext = nullptr;

    ComPtr<ID3D11InputLayout> mLayout;
    ComPtr<ID3D11RasterizerState> mRastState;

    ComPtr<ID3D11VertexShader> mVertexShader;
    ComPtr<ID3D11PixelShader> mPixelShader;

    ComPtr<ID3D11Buffer> mVertexBuffer;
    ComPtr<ID3D11Buffer> mIndexBuffer;
    ComPtr<ID3D11Buffer> mConstantBuffer;
    ComPtr<ID3D11Buffer> mMaterialBuffer;
    ComPtr<ID3D11Buffer> mLightBuffer;

    ComPtr<ID3D11ShaderResourceView> mTextureSRV;
    ComPtr<ID3D11SamplerState> mSamplerState;

    UINT mIndexCount = 0;
    UINT mStride = sizeof(Vertex);
    UINT mOffset = 0;

    bool mVisible = true;

    struct CBMatrix
    {
        Matrix matrix;
    };

    struct CBMaterial
    {
        int hasTexture;
        float padding[3];
    };

    struct CBLight
    {
        Vector3 lightDir;
        float padding1;

        Vector3 cameraPos;
        float intensity;
    };
};
