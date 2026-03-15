#include "Display\DisplayWin32.h"
#include "Display\InputDevice.h"
#include "Display\Keys.h"
#include "Game.h"
#include "GameComponent.h"

Game::Game(int width, int height)
    : clientWidth(width),
    clientHeight(height)
{
}

bool Game::Initialize()
{
    display = std::make_unique<DisplayWin32>(
        this,
        L"My3DApp",
        clientWidth,
        clientHeight);

    if (!display->Initialize())
        return false;

    display->Show();

    input = std::make_unique<InputDevice>(this);

    if (!InitializeD3D())
        return false;

    for (auto& c : components)
        c->Initialize();

    prevTime = std::chrono::steady_clock::now();

    return true;
}

bool Game::InitializeD3D()
{
    DXGI_SWAP_CHAIN_DESC swapDesc{};
    swapDesc.BufferCount = 2;
    swapDesc.BufferDesc.Width = clientWidth;
    swapDesc.BufferDesc.Height = clientHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = display->GetHWND();
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
        &swapChain,
        &device,
        nullptr,
        &context);

    if (FAILED(res))
        return false;

    CreateBackBuffer();
    return true;
}

void Game::CreateBackBuffer()
{
    ComPtr<ID3D11Texture2D> backTex;

    swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        &backTex);

    device->CreateRenderTargetView(
        backTex.Get(),
        nullptr,
        &backBuffer);
}

void Game::Run()
{
    MSG msg{};

    while (!exitRequested)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                exitRequested = true;
        }

        if (input->IsKeyDown(static_cast<int>(Keys::Escape))) {
            Exit();
        }

        auto curTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(curTime - prevTime).count();
        prevTime = curTime;

        Update(deltaTime);
        Draw();
        EndFrame();
    }
}

void Game::Update(float deltaTime)
{
    totalTime += deltaTime;
    frameCount++;

    if (totalTime > 1.0f)
    {
        float fps = frameCount / totalTime;

        wchar_t text[256];
        swprintf_s(text, L"FPS: %.2f", fps);

        SetWindowText(display->GetHWND(), text);

        totalTime -= 1.0f;
        frameCount = 0;
    }

    for (auto& c : components)
        c->Update(deltaTime);

    input->EndFrame();
}

void Game::Draw()
{
    D3D11_VIEWPORT viewport {};
    viewport.Width = static_cast<float>(clientWidth);
    viewport.Height = static_cast<float>(clientHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    float color[] = { 0.f, 0.f, 0.f, 1.0f };

    context->RSSetViewports(1, &viewport);
    context->OMSetRenderTargets(1, backBuffer.GetAddressOf(), nullptr);
    context->ClearRenderTargetView(backBuffer.Get(), color);

    for (auto& c : components)
        c->Draw();
}

void Game::AddComponent(std::unique_ptr<GameComponent> component)
{
    components.push_back(std::move(component));
}

void Game::EndFrame()
{
    swapChain->Present(1, 0);
}

void Game::Exit()
{
    exitRequested = true;
}
