#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>
#include <DDSTextureLoader.h>
#include <iostream>

#include "Engine\Camera\CameraBase.h"
#include "Engine\GameApp.h"
#include "Engine\Graphics\Mesh\ModelImporter.h"
#include "RenderableComponent.h"

RenderableComponent::RenderableComponent(GameApp* gameApp) :
	GameComponent(gameApp),
	mDevice(mGameApp->GetDevice()),
	mContext(mGameApp->GetContext()),
	mTransform(std::make_unique<TransformComponent>())
{
}

RenderableComponent::RenderableComponent(GameApp* gameApp, const std::string& modelFile, const std::string& textureFile)
	: GameComponent(gameApp),
	mDevice(mGameApp->GetDevice()),
	mContext(mGameApp->GetContext()),
	mTransform(std::make_unique<TransformComponent>())
{
	if (!ModelImporter::LoadModel(modelFile, mPoints, mIndices))
	{
		std::cout << "Failed to load model: " << modelFile << std::endl;
	}

	std::wstring wTextureFile(textureFile.begin(), textureFile.end());
	DirectX::CreateDDSTextureFromFile(
		mDevice,
		wTextureFile.c_str(),
		nullptr,
		mTextureSRV.GetAddressOf());

	mIndexCount = static_cast<UINT>(mIndices.size());
}

void RenderableComponent::Initialize()
{
	CreateConstantBuffer();
	CreateMaterialBuffer();

	if (mTextureSRV)
	{
		CreateSamplerState();
	}

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
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mDevice->CreateInputLayout(
		inputDesc,
		3,
		vertexCodeBuffer->GetBufferPointer(),
		vertexCodeBuffer->GetBufferSize(),
		&mLayout);
}

void RenderableComponent::CreateGeometry()
{
	GetMesh(mPoints, mIndices);

	mIndexCount = static_cast<UINT>(mIndices.size());
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

void RenderableComponent::CreateMaterialBuffer()
{
	D3D11_BUFFER_DESC materialBufferDesc{};
	materialBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	materialBufferDesc.ByteWidth = sizeof(CBMaterial);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	mDevice->CreateBuffer(&materialBufferDesc, nullptr, &mMaterialBuffer);
}

void RenderableComponent::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	mDevice->CreateSamplerState(&samplerDesc, &mSamplerState);
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
	CBMatrix bufferMatrix {};
	CameraBase* camera = mGameApp->GetCamera();

	Matrix worldMatrix = mTransform->GetWorldMatrix();
	Matrix worldViewProjection = worldMatrix * camera->GetView() * camera->GetProjection();
	bufferMatrix.matrix = DirectX::XMMatrixTranspose(worldViewProjection);

	mContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &bufferMatrix, 0, 0);
	mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	CBMaterial materialBuffer{};
	materialBuffer.hasTexture = (mTextureSRV != nullptr);

	mContext->UpdateSubresource(mMaterialBuffer.Get(), 0, nullptr, &materialBuffer, 0, 0);
	mContext->PSSetConstantBuffers(1, 1, mMaterialBuffer.GetAddressOf());

	mContext->IASetInputLayout(mLayout.Get());
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mStride, &mOffset);
	mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
	mContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

	if (mTextureSRV)
	{
		ID3D11SamplerState* samplers[] = { mSamplerState.Get() };
		mContext->PSSetSamplers(0, 1, samplers);
		mContext->PSSetShaderResources(0, 1, mTextureSRV.GetAddressOf());
	}

	mContext->RSSetState(mRastState.Get());

	mContext->DrawIndexed(mIndexCount, 0, 0);
}
