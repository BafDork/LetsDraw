#pragma once

#include "GameComponent.h"
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class TriangleComponent : public GameComponent
{
public:
    explicit TriangleComponent(Game* game);
    ~TriangleComponent() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void DestroyResources() override;

private:
    void CreateShaders();
    void CreateGeometry();
    void CreateRasterizerState();

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;

    UINT stride = 32;
    UINT offset = 0;
};
