#pragma comment(lib, "d3dcompiler.lib")

#include "Engine\GameApp.h"
#include "RenderableComponent.h"

#include <d3dcompiler.h>
#include <iostream>

RenderableComponent::RenderableComponent(
	GameApp* gameApp,
	std::vector<Vertex> points,
	std::vector<uint32_t> indices)
	: GameComponent(gameApp),
	mPoints(std::move(points)),
	mIndices(std::move(indices)),
	mDevice(mGameApp->GetDevice()),
	mContext(mGameApp->GetContext()),
	mTransform(std::make_unique<TransformComponent>()),
	mIndexCount(static_cast<UINT>(mIndices.size()))
{
}

void RenderableComponent::Initialize()
{
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
			<< (char*) errorMessage->GetBufferPointer()
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
			<< (char*) errorMessage->GetBufferPointer()
			<< std::endl;
		return;
	}

	mDevice->CreateVertexShader(
		vertexCodeBuffer->GetBufferPointer(),
		vertexCodeBuffer->GetBufferSize(),
		nullptr,
		&mVertexShader);

	mDevice->CreatePixelShader(
		pixelCodeBuffer->GetBufferPointer(),
		pixelCodeBuffer->GetBufferSize(),
		nullptr,
		&mPixelShader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mDevice->CreateInputLayout(
		inputDesc,
		2,
		vertexCodeBuffer->GetBufferPointer(),
		vertexCodeBuffer->GetBufferSize(),
		&mLayout);
}

void RenderableComponent::CreateGeometry()
{
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * mPoints.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData{};
	vertexBufferData.pSysMem = mPoints.data();

	mDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mVertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * mIndices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferData{};
	indexBufferData.pSysMem = mIndices.data();

	mDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &mIndexBuffer);
}

void RenderableComponent::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(CBMatrix);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;

	mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer);
}

void RenderableComponent::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rastDesc{};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	mDevice->CreateRasterizerState(&rastDesc, &mRastState);
}

void RenderableComponent::Draw()
{
	CBMatrix bufferMatrix{};
	bufferMatrix.matrix = DirectX::XMMatrixTranspose(mTransform->GetWorldMatrix());
	mContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &bufferMatrix, 0, 0);
	mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	mContext->IASetInputLayout(mLayout.Get());
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mStride, &mOffset);
	mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	mContext->RSSetState(mRastState.Get());

	mContext->DrawIndexed(mIndexCount, 0, 0);
}
