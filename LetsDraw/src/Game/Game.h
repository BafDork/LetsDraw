#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "Display\DisplayWin32.h"
#include "Display\InputDevice.h"
#include "Graphics\RenderableComponent.h"
#include "RacketComponent.h"
#include "BallComponent.h"

#include <chrono>
#include <d3d11.h>
#include <DirectXCollision.h>
#include <dxgi.h>
#include <memory>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;
using DirectX::BoundingBox;

class GameComponent;

class Game
{
public:
    Game(int width, int height);
    ~Game() = default;

    bool Initialize();
    void Run();
    void Exit();

    ID3D11Device* GetDevice() const { return device.Get(); }
    ID3D11DeviceContext* GetContext() const { return context.Get(); }
    DisplayWin32* GetDisplay() const { return display.get(); }
    InputDevice* GetInput() const { return input.get(); }

private:
    bool InitializeD3D();
    void CreateBackBuffer();
	void CreateGameComponents();
    void Update(float deltaTime);
    void Draw();
    void EndFrame();

    void AddComponent(GameComponent* component);

private:
    int clientWidth;
    int clientHeight;

    std::unique_ptr<DisplayWin32> display;
    std::unique_ptr<InputDevice> input;

    //TODO Refactor to uniq_ptr
    std::vector<GameComponent*> components;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> backBuffer;

    std::unique_ptr<RacketComponent> leftRacket;
    std::unique_ptr<RacketComponent> rightRacket;
    std::unique_ptr<BallComponent> ball;

    BoundingBox topWall;
    BoundingBox bottomWall;
    BoundingBox leftWall;
    BoundingBox rightWall;

    int leftScore = 0;
    int rightScore = 0;
    float ballSpeedMultiplier = 1.05f;

    bool exitRequested = false;

    std::chrono::steady_clock::time_point prevTime;
    float totalTime = 0.0f;
    unsigned int frameCount = 0;
};
