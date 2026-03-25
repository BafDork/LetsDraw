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

protected:
    virtual void CreateShaders();
    virtual void CreateGeometry();
    virtual void CreateRasterizerState();
	virtual void CreateConstantBuffer();
    virtual void CreateMaterialBuffer();
    virtual void CreateSamplerState();

    virtual void GetMesh(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices) {}

private:
    std::vector<Vertex> mPoints;
    std::vector<uint32_t> mIndices;

    std::unique_ptr<TransformComponent> mTransform;

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

    ComPtr<ID3D11ShaderResourceView> mTextureSRV;
    ComPtr<ID3D11SamplerState> mSamplerState;

    UINT mIndexCount;
    UINT mStride = sizeof(Vertex);
    UINT mOffset = 0;

    struct CBMatrix
    {
        Matrix matrix;
    };

    struct CBMaterial
    {
        int hasTexture;
        float padding[3];
    };
};
