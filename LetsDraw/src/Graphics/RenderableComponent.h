#pragma once
#pragma comment(lib, "d3d11.lib")

#include "Engine\GameComponent.h"
#include "TransformComponent.h"
#include "Vertex.h"

#include <d3d11.h>
#include <memory>
#include <SimpleMath.h>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;

class GameApp;
class CameraComponent;

class RenderableComponent : public GameComponent
{
public:
    RenderableComponent(
        GameApp* gameApp,
        std::vector<Vertex> points,
        std::vector<uint32_t> indices);

    void Initialize() override;
    void Draw() override;

    TransformComponent* GetTransform() const { return mTransform.get(); }
    void SetTransform(std::unique_ptr<TransformComponent> transform) { mTransform = std::move(transform); }

protected:
    virtual void CreateShaders();
    virtual void CreateGeometry();
    virtual void CreateRasterizerState();
	virtual void CreateConstantBuffer();

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

    UINT mIndexCount;
    UINT mStride = sizeof(Vertex);
    UINT mOffset = 0;

    struct CBMatrix
    {
        Matrix matrix;
    };
};
