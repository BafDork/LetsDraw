#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <memory>
#include <chrono>

class DisplayWin32;
class InputDevice;
class GameComponent;

class Game
{
public:
    Game(int width, int height);
    ~Game();

    bool Initialize();
    void Run();
    void Exit();

    void OnKeyDown(unsigned int key);

    ID3D11Device* GetDevice() const { return device.Get(); }
    ID3D11DeviceContext* GetContext() const { return context.Get(); }
    DisplayWin32* GetDisplay() const { return display.get(); }
    InputDevice* GetInput() const { return input.get(); }

    void AddComponent(std::unique_ptr<GameComponent> component);

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

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBuffer;

    bool exitRequested = false;

    std::chrono::steady_clock::time_point prevTime;
    float totalTime = 0.0f;
    unsigned int frameCount = 0;
};
