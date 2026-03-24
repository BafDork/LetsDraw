#pragma once

#include <windows.h>
#include <string>

class GameApp;

class DisplayWin32
{
public:
    DisplayWin32(
        GameApp* gameApp,
        const std::wstring& windowName,
        int width,
        int height);

    ~DisplayWin32();

    bool Initialize();
    void Show();

    HWND GetHWND() const { return mHWnd; }
    int GetClientWidth() const { return mClientWidth; }
    int GetClientHeight() const { return mClientHeight; }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    GameApp* mGameApp;

    std::wstring mWindowName;

    int mClientWidth;
    int mClientHeight;

    HINSTANCE mHInstance;
    HWND mHWnd;
};
