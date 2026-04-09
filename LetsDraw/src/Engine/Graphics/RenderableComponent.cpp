#include <DDSTextureLoader.h>
#include <iostream>

#include "Engine\Camera\CameraBase.h"
#include "Engine\GameApp.h"
#include "Engine\Graphics\Mesh\ModelImporter.h"
#include "LightComponent.h"
#include "RenderableComponent.h"

RenderableComponent::RenderableComponent(GameApp* gameApp) :
	GameComponent(gameApp),
	mDevice(mGameApp->GetDevice()),
	mContext(mGameApp->GetContext()),
	mTransform(std::make_unique<TransformComponent>())
{
}

RenderableComponent::RenderableComponent(GameApp* gameApp,
	const std::string& modelFile,
	const std::string& textureFile)
	: GameComponent(gameApp),
	mDevice(mGameApp->GetDevice()),
	mContext(mGameApp->GetContext()),
	mTransform(std::make_unique<TransformComponent>())
{
	if (!ModelImporter::LoadModel(modelFile, mPoints, mIndices))
	{
		std::cout << "Failed to load model: " << modelFile << std::endl;
	}

	LoadTexture(textureFile, mTextureSRV);

	mIndexCount = static_cast<UINT>(mIndices.size());
}

bool RenderableComponent::LoadTexture(const std::string& textureFile, ComPtr<ID3D11ShaderResourceView>& outSRV)
{
	std::wstring wTextureFile(textureFile.begin(), textureFile.end());
	HRESULT hr = DirectX::CreateDDSTextureFromFile(
		mDevice,
		wTextureFile.c_str(),
		nullptr,
		outSRV.GetAddressOf()
	);

	if (FAILED(hr))
	{
		std::cout << "Failed to load texture: " << textureFile << std::endl;
		return false;
	}

	return true;
}

void RenderableComponent::Initialize()
{
	CreateConstantBuffer();
	CreateMaterialBuffer();
	CreateLightBuffer();

	if (mTextureSRV)
	{
		CreateSamplerState();
	}

	D3D11_SAMPLER_DESC desc{};
	desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;

	HRESULT hr = mDevice->CreateSamplerState(&desc, &mShadowSampler);

	CreateShaders();
	CreateGeometry();
	CreateRasterizerState();
}

void RenderableComponent::CreateShaders()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			  
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT,
		  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mBaseShader = std::make_unique<Shader>();
	mBaseShader->Load(
		mDevice,
		L"./Shaders/BaseShader.hlsl",
		"VSMain",
		"PSMain",
		inputDesc,
		4);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	mShadowShader = std::make_unique<Shader>();
	mShadowShader->Load(
		mDevice,
		L"./Shaders/ShadowShader.hlsl",
		"VSMain",
		"",
		inputDesc,
		4);
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
	mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mLightProjBuffer);
}

void RenderableComponent::CreateMaterialBuffer()
{
	D3D11_BUFFER_DESC materialBufferDesc{};
	materialBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	materialBufferDesc.ByteWidth = sizeof(CBMaterial);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	mDevice->CreateBuffer(&materialBufferDesc, nullptr, &mMaterialBuffer);
}

void RenderableComponent::CreateLightBuffer()
{
	D3D11_BUFFER_DESC lightBufferDesc{};
	lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lightBufferDesc.ByteWidth = sizeof(CBLight);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	mDevice->CreateBuffer(&lightBufferDesc, nullptr, &mLightBuffer);
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

void RenderableComponent::DrawShadow()
{
	LightComponent* light = mGameApp->GetMainLight();
	if (!light) return;

	Matrix world = mTransform->GetWorldMatrix();
	Matrix wvp = world * light->GetLightViewProj();

	CBMatrix cb{};
	cb.worldViewProj = XMMatrixTranspose(wvp);

	mContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	mContext->VSSetShader(mShadowShader->GetVS(), nullptr, 0);
	mContext->PSSetShader(nullptr, nullptr, 0);

	mContext->IASetInputLayout(mShadowShader->GetLayout());
	mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mStride, &mOffset);
	mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mContext->DrawIndexed(mIndexCount, 0, 0);
}

void RenderableComponent::Draw()
{
	if (!mVisible)
		return;

	CBMatrix bufferMatrix {};
	CameraBase* camera = mGameApp->GetCamera();

	Matrix worldMatrix = mTransform->GetWorldMatrix();
	Matrix worldViewProjection = worldMatrix * camera->GetView() * camera->GetProjection();
	bufferMatrix.world = XMMatrixTranspose(worldMatrix);
	bufferMatrix.worldViewProj = XMMatrixTranspose(worldViewProjection);

	mContext->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &bufferMatrix, 0, 0);
	mContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

	CBMaterial materialBuffer{};
	materialBuffer.hasTexture = (mTextureSRV != nullptr);

	mContext->UpdateSubresource(mMaterialBuffer.Get(), 0, nullptr, &materialBuffer, 0, 0);
	mContext->PSSetConstantBuffers(1, 1, mMaterialBuffer.GetAddressOf());

	CBLight lightBuffer{};
	LightComponent* light = mGameApp->GetMainLight();

	lightBuffer.lightDir = light->GetDirection();
	lightBuffer.intensity = light->GetIntensity();
	lightBuffer.cameraPos = mGameApp->GetCamera()->GetTransform()->GetPosition();

	mContext->UpdateSubresource(mLightBuffer.Get(), 0, nullptr, &lightBuffer, 0, 0);
	mContext->PSSetConstantBuffers(2, 1, mLightBuffer.GetAddressOf());

	CBMatrix lightMatrix{};
	Matrix wvp = worldMatrix * light->GetLightViewProj();
	lightMatrix.worldViewProj = XMMatrixTranspose(wvp);

	mContext->UpdateSubresource(mLightProjBuffer.Get(), 0, nullptr, &lightMatrix, 0, 0);
	mContext->VSSetConstantBuffers(3, 1, mLightProjBuffer.GetAddressOf());

	mContext->IASetInputLayout(mBaseShader->GetLayout());
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mStride, &mOffset);
	mContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	mContext->VSSetShader(mBaseShader->GetVS(), nullptr, 0);
	mContext->PSSetShader(mBaseShader->GetPS(), nullptr, 0);

	if (mTextureSRV)
	{
		ID3D11SamplerState* samplers[] = { mSamplerState.Get() };
		mContext->PSSetSamplers(0, 1, samplers);
		mContext->PSSetShaderResources(0, 1, mTextureSRV.GetAddressOf());
	}

	ID3D11SamplerState* samplers2[] = { mShadowSampler.Get() };
	mContext->PSSetSamplers(1, 1, samplers2);
	ID3D11ShaderResourceView* shadowSRV = light->GetShadowSRV();
	mContext->PSSetShaderResources(1, 1, &shadowSRV);

	mContext->RSSetState(mRastState.Get());

	mContext->DrawIndexed(mIndexCount, 0, 0);
}
