#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "Display\InputDevice.h"
#include "Display\DisplayWin32.h"

#include <chrono>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class GameComponent;

class Game
{
public:
    Game(int width, int height);
    ~Game() = default;

    bool Initialize();
    void Run();
    void Exit();

    void AddComponent(std::unique_ptr<GameComponent> component);

    ID3D11Device* GetDevice() const { return device.Get(); }
    ID3D11DeviceContext* GetContext() const { return context.Get(); }
    DisplayWin32* GetDisplay() const { return display.get(); }
    InputDevice* GetInput() const { return input.get(); }

private:
    bool InitializeD3D();
    void CreateBackBuffer();
    void Update(float deltaTime);
    void Draw();
    void EndFrame();

private:
    int clientWidth;
    int clientHeight;

    std::unique_ptr<DisplayWin32> display;
    std::unique_ptr<InputDevice> input;

    std::vector<std::unique_ptr<GameComponent>> components;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> backBuffer;

    bool exitRequested = false;

    std::chrono::steady_clock::time_point prevTime;
    float totalTime = 0.0f;
    unsigned int frameCount = 0;
};
