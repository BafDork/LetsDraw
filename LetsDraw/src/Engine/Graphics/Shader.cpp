#pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>
#include <iostream>

#include "Shader.h"

bool Shader::Load(
    ID3D11Device* device,
    const std::wstring& file,
    const std::string& vsEntry,
    const std::string& psEntry,
    D3D11_INPUT_ELEMENT_DESC* layout,
    UINT layoutCount)
{
    ComPtr<ID3DBlob> vsBuf;
    ComPtr<ID3DBlob> psBuf;
    ComPtr<ID3DBlob> error;

    HRESULT hr = D3DCompileFromFile(
        file.c_str(),
        nullptr,
        nullptr,
        vsEntry.c_str(),
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &vsBuf,
        &error);

    if (FAILED(hr))
    {
        if (error)
            std::cout << (char*)error->GetBufferPointer();
        return false;
    }

    device->CreateVertexShader(
        vsBuf->GetBufferPointer(),
        vsBuf->GetBufferSize(),
        nullptr,
        &mVS);

    if (!psEntry.empty())
    {
        hr = D3DCompileFromFile(
            file.c_str(),
            nullptr,
            nullptr,
            psEntry.c_str(),
            "ps_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            &psBuf,
            &error);

        if (FAILED(hr))
        {
            if (error)
                std::cout << (char*)error->GetBufferPointer();
            return false;
        }

        device->CreatePixelShader(
            psBuf->GetBufferPointer(),
            psBuf->GetBufferSize(),
            nullptr,
            &mPS);
    }

    device->CreateInputLayout(
        layout,
        layoutCount,
        vsBuf->GetBufferPointer(),
        vsBuf->GetBufferSize(),
        &mLayout);

    return true;
}
