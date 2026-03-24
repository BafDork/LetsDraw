#include <iostream>

#include "Engine\Camera\OrthographicCameraBase.h"
#include "Engine\Graphics\TransformComponent.h"
#include "Engine\Window\Keys.h"
#include "GameApp.h"

GameApp::GameApp(int width, int height) : 
    mClientWidth(width),
    mClientHeight(height)
{
}

bool GameApp::Initialize()
{
    mDisplay = std::make_unique<DisplayWin32>(
        this,
        L"My3DApp",
        mClientWidth,
        mClientHeight);

    if (!mDisplay->Initialize())
        return false;

    mInput = std::make_unique<InputDevice>(this);

    if (!InitializeD3D())
        return false;

    CreateCamera();
    OnCreateGame();

    for (auto& component : mComponents)
    {
        component->Initialize();
    }

    mDisplay->Show();

    mPrevTime = std::chrono::steady_clock::now();

    return true;
}

bool GameApp::InitializeD3D()
{
    DXGI_SWAP_CHAIN_DESC swapDesc{};
    swapDesc.BufferCount = 2;
    swapDesc.BufferDesc.Width = mClientWidth;
    swapDesc.BufferDesc.Height = mClientHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = mDisplay->GetHWND();
    swapDesc.SampleDesc.Count = 1;
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapDesc,
        &mSwapChain,
        &mDevice,
        nullptr,
        &mContext);

    if (FAILED(res))
    {
        return false;
    }

    CreateBackBuffer();
    CreateDepthStencil();

    return true;
}

void GameApp::CreateBackBuffer()
{
    ComPtr<ID3D11Texture2D> backTex;

    mSwapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        &backTex);

    mDevice->CreateRenderTargetView(
        backTex.Get(),
        nullptr,
        &mBackBuffer);
}

void GameApp::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width = mClientWidth;
    depthDesc.Height = mClientHeight;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags = 0;
    depthDesc.MiscFlags = 0;

    mDevice->CreateTexture2D(&depthDesc, nullptr, &mDepthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc{};
    depthViewDesc.Format = depthDesc.Format;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthViewDesc.Texture2D.MipSlice = 0;

    mDevice->CreateDepthStencilView(mDepthStencil.Get(), &depthViewDesc, &mDepthView);

    D3D11_DEPTH_STENCIL_DESC dsDesc{};
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dsDesc.StencilEnable = false;

    mDevice->CreateDepthStencilState(&dsDesc, &mDepthState);
}

void GameApp::CreateCamera()
{
    auto orthoCamera = std::make_unique<OrthographicCameraBase>(
        this, static_cast<float>(mClientWidth), static_cast<float>(mClientHeight));
    orthoCamera->GetTransform()->SetPosition({ 0, 0, -10 });
    mCamera = orthoCamera.get();

    AddComponent(std::move(orthoCamera));
}

void GameApp::Run()
{
    MSG msg{};

    while (!mExitRequested)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                mExitRequested = true;
        }

        if (mInput->IsKeyDown(static_cast<int>(Keys::Escape))) 
        {
            Exit();
        }

        auto curTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(curTime - mPrevTime).count();
        mPrevTime = curTime;

        Update(deltaTime);
        Draw();
        EndFrame();
    }
}

void GameApp::Update(float deltaTime)
{
    mTotalTime += deltaTime;
    mFrameCount++;

    if (mTotalTime > 1.0f)
    {
        float fps = mFrameCount / mTotalTime;

        wchar_t text[256];
        swprintf_s(text, L"FPS: %.2f", fps);

        SetWindowText(mDisplay->GetHWND(), text);

        mTotalTime -= 1.0f;
        mFrameCount = 0;
    }

    OnUpdate(deltaTime);

    for (auto& component : mComponents)
    {
        component->Update(deltaTime);
    }

    mInput->EndFrame();
}

void GameApp::Draw()
{
    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(mClientWidth);
    viewport.Height = static_cast<float>(mClientHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    float color[] = { 0.f, 0.f, 0.f, 1.0f };

    mContext->ClearRenderTargetView(mBackBuffer.Get(), color);
    mContext->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    mContext->RSSetViewports(1, &viewport);
    mContext->OMSetRenderTargets(1, mBackBuffer.GetAddressOf(), mDepthView.Get());
    mContext->OMSetDepthStencilState(mDepthState.Get(), 0);

    for (auto& component : mComponents)
    {
        component->Draw();
    }
}

void GameApp::AddComponent(std::unique_ptr<GameComponent> component)
{
    mComponents.push_back(std::move(component));
}

void GameApp::EndFrame()
{
    mSwapChain->Present(1, 0);
}

void GameApp::Exit()
{
    mExitRequested = true;
}
