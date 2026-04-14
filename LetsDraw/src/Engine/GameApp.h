#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "Engine\Graphics\Light\BaseLightComponent.h"
#include "Engine\Graphics\Light\DirectionalLightComponent.h"
#include "Engine\Window\DisplayWin32.h"
#include "Engine\Window\InputDevice.h"
#include "GameComponent.h"

#include <chrono>
#include <d3d11.h>
#include <memory>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class GameComponent;
class CameraBase;

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
    CameraBase* GetCamera() const { return mCamera; }
	
    int GetClientWidth() const { return mClientWidth; }
	int GetClientHeight() const { return mClientHeight; }
    
    void SetMainLight(DirectionalLightComponent* light) { mMainLight = light; AddLight(light); }
    DirectionalLightComponent* GetMainLight() const { return mMainLight; }

    const std::vector<BaseLightComponent*>& GetLights() const { return mLights; }

protected:
    virtual void CreateCamera();
    virtual void CreateMainLight();
    virtual void OnCreateGame() {}
    virtual void OnUpdate(float deltaTime) {}

    void AddComponent(std::unique_ptr<GameComponent> component) { mComponents.push_back(std::move(component)); }
    void AddLight(BaseLightComponent* light) { mLights.push_back(light); }

protected:
    int mClientWidth;
    int mClientHeight;

    std::unique_ptr<DisplayWin32> mDisplay;
    std::unique_ptr<InputDevice> mInput;

    std::vector<std::unique_ptr<GameComponent>> mComponents;
    std::vector<BaseLightComponent*> mLights;

    ComPtr<ID3D11Device> mDevice;
    ComPtr<ID3D11DeviceContext> mContext;
    ComPtr<IDXGISwapChain> mSwapChain;
    ComPtr<ID3D11RenderTargetView> mBackBuffer;

    ComPtr<ID3D11Texture2D> mDepthStencil;
    ComPtr<ID3D11DepthStencilView> mDepthView;
    ComPtr<ID3D11DepthStencilState> mDepthState;

    CameraBase* mCamera = nullptr;
    DirectionalLightComponent* mMainLight = nullptr;

private:
    bool InitializeD3D();
    void CreateBackBuffer();
	void CreateDepthStencil();

    void Update(float deltaTime);
    void Draw();
    void DrawShadow();
    void EndFrame();

private:
    bool mExitRequested = false;

    std::chrono::steady_clock::time_point mPrevTime;
    float mTotalTime = 0.f;
    unsigned mFrameCount = 0;
};
