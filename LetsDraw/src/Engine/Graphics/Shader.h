#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>

using Microsoft::WRL::ComPtr;

class Shader
{
public:
    bool Load(
        ID3D11Device* device,
        const std::wstring& file,
        const std::string& vsEntry,
        const std::string& psEntry,
        D3D11_INPUT_ELEMENT_DESC* layout,
        UINT layoutCount);

    ID3D11VertexShader* GetVS() const { return mVS.Get(); }
    ID3D11PixelShader* GetPS() const { return mPS.Get(); }
    ID3D11InputLayout* GetLayout() const { return mLayout.Get(); }

private:
    ComPtr<ID3D11VertexShader> mVS;
    ComPtr<ID3D11PixelShader> mPS;
    ComPtr<ID3D11InputLayout> mLayout;
};
