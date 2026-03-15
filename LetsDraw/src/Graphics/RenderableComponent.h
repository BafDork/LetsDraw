#pragma once
#pragma comment(lib, "d3d11.lib")

#include "Game\GameComponent.h"
#include "Vertex.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class TransformComponent;

class RenderableComponent : public GameComponent
{
public:
    RenderableComponent(Game* game,
        std::vector<Vertex> pts,
        std::vector<uint32_t> idx);

    RenderableComponent(Game* game,
        std::unique_ptr<TransformComponent> t,
        std::vector<Vertex> pts,
        std::vector<uint32_t> idx);

    ~RenderableComponent() override = default;

    void Initialize() override;
    void Draw() override;

    static std::unique_ptr<RenderableComponent> CreateQuadComponent(Game* game);

protected:
    virtual void CreateShaders();
    virtual void CreateGeometry();
    virtual void CreateRasterizerState();
	virtual void CreateConstantBuffer();

    std::unique_ptr<TransformComponent> transform;

private:
    std::vector<Vertex> points;
    std::vector<uint32_t> indices;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    ComPtr<ID3D11InputLayout> layout;
    ComPtr<ID3D11RasterizerState> rastState;

    ComPtr<ID3D11VertexShader> vertexShader;
    ComPtr<ID3D11PixelShader> pixelShader;

    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;
    ComPtr<ID3D11Buffer> constantBuffer;

    UINT indexCount;
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    struct CBMatrix
    {
        DirectX::XMMATRIX matrix;
    };
};
