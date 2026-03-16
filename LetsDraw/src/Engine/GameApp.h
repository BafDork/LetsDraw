#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "Window\DisplayWin32.h"
#include "Window\InputDevice.h"

#include <chrono>
#include <d3d11.h>
#include <memory>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class GameComponent;

class GameApp
{
public:
    GameApp(int width, int height);

    bool Initialize();
    void Run();
    void Exit();

    ID3D11Device* GetDevice() const { return mDevice.Get(); }
    ID3D11DeviceContext* GetContext() const { return mContext.Get(); }
    DisplayWin32* GetDisplay() const { return mDisplay.get(); }
    InputDevice* GetInput() const { return mInput.get(); }

protected:
    virtual void OnCreateGame() {}
    virtual void OnUpdate(float deltaTime) {}

    void AddComponent(std::unique_ptr<GameComponent> component);

protected:
    int mClientWidth;
    int mClientHeight;

    std::unique_ptr<DisplayWin32> mDisplay;
    std::unique_ptr<InputDevice> mInput;

    std::vector<std::unique_ptr<GameComponent>> mComponents;

    ComPtr<ID3D11Device> mDevice;
    ComPtr<ID3D11DeviceContext> mContext;
    ComPtr<IDXGISwapChain> mSwapChain;
    ComPtr<ID3D11RenderTargetView> mBackBuffer;

private:
    bool InitializeD3D();
    void CreateBackBuffer();

    void Update(float deltaTime);
    void Draw();
    void EndFrame();

private:
    bool mExitRequested = false;

    std::chrono::steady_clock::time_point mPrevTime;
    float mTotalTime = 0.f;
    unsigned mFrameCount = 0;
};
