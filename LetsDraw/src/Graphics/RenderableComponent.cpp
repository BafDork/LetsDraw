#pragma comment(lib, "d3dcompiler.lib")

#include "Game\Game.h"
#include "Game\TransformComponent.h"
#include "RenderableComponent.h"

#include <d3dcompiler.h>
#include <iostream>

RenderableComponent::RenderableComponent(Game* game,
	std::vector<Vertex> pts,
	std::vector<uint32_t> idx)
	: RenderableComponent(game,
		std::make_unique<TransformComponent>(),
		std::move(pts),
		std::move(idx))
{
}

RenderableComponent::RenderableComponent(Game* game,
	std::unique_ptr<TransformComponent> t,
	std::vector<Vertex> pts,
	std::vector<uint32_t> idx)
	: GameComponent(game),
	transform(std::move(t)),
	points(std::move(pts)),
	indices(std::move(idx)),
	indexCount(static_cast<UINT>(indices.size()))
{
}

void RenderableComponent::Initialize()
{
	device = game->GetDevice();
	context = game->GetContext();

	CreateConstantBuffer();
	CreateShaders();
	CreateGeometry();
	CreateRasterizerState();
}

void RenderableComponent::CreateShaders()
{
	ComPtr<ID3DBlob> vertexCodeBuffer = nullptr;
	ComPtr<ID3DBlob> pixelCodeBuffer = nullptr;
	ComPtr<ID3DBlob> errorMessage = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"./Shaders/DefaultShader.hlsl",
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexCodeBuffer,
		&errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			std::cout << "Error compiling vertex shader: "
			<< (char*)errorMessage->GetBufferPointer()
			<< std::endl;
		return;
	}

	result = D3DCompileFromFile(
		L"./Shaders/DefaultShader.hlsl",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelCodeBuffer,
		&errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			std::cout << "Error compiling pixel shader: "
			<< (char*)errorMessage->GetBufferPointer()
			<< std::endl;
		return;
	}

	device->CreateVertexShader(
		vertexCodeBuffer->GetBufferPointer(),
		vertexCodeBuffer->GetBufferSize(),
		nullptr,
		&vertexShader);

	device->CreatePixelShader(
		pixelCodeBuffer->GetBufferPointer(),
		pixelCodeBuffer->GetBufferSize(),
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
		vertexCodeBuffer->GetBufferPointer(),
		vertexCodeBuffer->GetBufferSize(),
		&layout);
}

void RenderableComponent::CreateGeometry()
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * points.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData{};
	vertexBufferData.pSysMem = points.data();

	device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData{};
	indexBufferData.pSysMem = indices.data();

	device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
}

void RenderableComponent::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(CBMatrix);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
}

void RenderableComponent::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rastDesc{};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, &rastState);
}

void RenderableComponent::Draw()
{
	CBMatrix bufferMatrix{};
	bufferMatrix.matrix = DirectX::XMMatrixTranspose(transform->GetWorldMatrix());
	context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &bufferMatrix, 0, 0);
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	context->IASetInputLayout(layout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);

	context->RSSetState(rastState.Get());

	context->DrawIndexed(indexCount, 0, 0);
}

std::unique_ptr<RenderableComponent> RenderableComponent::CreateQuadComponent(Game* game)
{
	std::vector<Vertex> quadVertices =
	{
		{{ 0.5f,  0.5f, 0.0f, 1.0f }, {1,1,1,1}},
		{{-0.5f,  0.5f, 0.0f, 1.0f }, {1,1,1,1}},
		{{-0.5f, -0.5f, 0.0f, 1.0f }, {1,1,1,1}},
		{{ 0.5f, -0.5f, 0.0f, 1.0f }, {1,1,1,1}}
	};

	std::vector<uint32_t> quadIndices = { 0,1,2, 0,2,3 };

	return std::make_unique<RenderableComponent>(
		game,
		std::move(quadVertices),
		std::move(quadIndices)
	);
}
