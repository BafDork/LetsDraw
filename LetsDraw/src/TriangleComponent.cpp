#include "TriangleComponent.h"
#include "Game.h"

#include <d3dcompiler.h>
#include <iostream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

TriangleComponent::TriangleComponent(Game* game)
    : GameComponent(game)
{
}

void TriangleComponent::Initialize()
{
    CreateShaders();
    CreateGeometry();
    CreateRasterizerState();
}

void TriangleComponent::CreateShaders()
{
    ID3DBlob* vertexBC = nullptr;
    ID3DBlob* pixelBC = nullptr;
    ID3DBlob* errorCode = nullptr;

    HRESULT res = D3DCompileFromFile(
        L"./Shaders/MyVeryFirstShader.hlsl",
        nullptr,
        nullptr,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &vertexBC,
        &errorCode);

    if (FAILED(res))
    {
        if (errorCode)
            std::cout << (char*)errorCode->GetBufferPointer() << std::endl;
        return;
    }

    res = D3DCompileFromFile(
        L"./Shaders/MyVeryFirstShader.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &pixelBC,
        &errorCode);

    if (FAILED(res))
    {
        if (errorCode)
            std::cout << (char*)errorCode->GetBufferPointer() << std::endl;
        return;
    }

    auto device = game->GetDevice();

    device->CreateVertexShader(
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        nullptr,
        &vertexShader);

    device->CreatePixelShader(
        pixelBC->GetBufferPointer(),
        pixelBC->GetBufferSize(),
        nullptr,
        &pixelShader);

    D3D11_INPUT_ELEMENT_DESC inputDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
          D3D11_APPEND_ALIGNED_ELEMENT,
          D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(
        inputDesc,
        2,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        &layout);

    vertexBC->Release();
    pixelBC->Release();
}

void TriangleComponent::CreateGeometry()
{
    XMFLOAT4 points[] =
    {
        { 0.5f,  0.5f, 0.5f, 1.0f }, {1,0,0,1},
        {-0.5f, -0.5f, 0.5f, 1.0f }, {0,0,1,1},
        { 0.5f, -0.5f, 0.5f, 1.0f }, {0,1,0,1},
        {-0.5f,  0.5f, 0.5f, 1.0f }, {1,1,1,1}
    };

    int indices[] = { 0,1,2, 1,0,3 };

    D3D11_BUFFER_DESC vbDesc{};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(points);
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData{};
    vbData.pSysMem = points;

    game->GetDevice()->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

    D3D11_BUFFER_DESC ibDesc{};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(indices);
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA ibData{};
    ibData.pSysMem = indices;

    game->GetDevice()->CreateBuffer(&ibDesc, &ibData, &indexBuffer);
}

void TriangleComponent::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rastDesc{};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    game->GetDevice()->CreateRasterizerState(&rastDesc, &rastState);
}

void TriangleComponent::Update(float)
{
    // Пока ничего
}

void TriangleComponent::Draw()
{
    auto context = game->GetContext();

    context->RSSetState(rastState.Get());
    context->IASetInputLayout(layout.Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->VSSetShader(vertexShader.Get(), nullptr, 0);
    context->PSSetShader(pixelShader.Get(), nullptr, 0);

    context->DrawIndexed(6, 0, 0);
}

void TriangleComponent::DestroyResources()
{
    vertexBuffer.Reset();
    indexBuffer.Reset();
    layout.Reset();
    vertexShader.Reset();
    pixelShader.Reset();
    rastState.Reset();
}
