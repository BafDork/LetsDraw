#pragma once
#pragma comment(lib, "d3d11.lib")

#include "Game\GameComponent.h"
#include "Game\TransformComponent.h"
#include "Vertex.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

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

    RenderableComponent(const RenderableComponent& other);
    RenderableComponent(RenderableComponent&& other) noexcept;

    ~RenderableComponent() override = default;

    void Initialize() override;
    void Draw() override;

    TransformComponent* GetTransform() const
    {
        return transform.get();
    }

    void SetTransform(std::unique_ptr<TransformComponent> t)
    {
        transform = std::move(t);
    }

protected:
    virtual void CreateShaders();
    virtual void CreateGeometry();
    virtual void CreateRasterizerState();
	virtual void CreateConstantBuffer();


private:
    std::vector<Vertex> points;
    std::vector<uint32_t> indices;

    std::unique_ptr<TransformComponent> transform;

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
