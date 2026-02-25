#pragma once

#include <windows.h>
#include <string>

class Game;

class DisplayWin32
{
public:
    DisplayWin32(Game* game,
        const std::wstring& windowName,
        int width,
        int height);

    ~DisplayWin32();

    bool Initialize();
    void Show();

    HWND GetHWND() const { return hWnd; }
    int GetClientWidth() const { return clientWidth; }
    int GetClientHeight() const { return clientHeight; }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    Game* game;

    std::wstring windowName;

    int clientWidth;
    int clientHeight;

    HINSTANCE hInstance;
    HWND hWnd;
};
