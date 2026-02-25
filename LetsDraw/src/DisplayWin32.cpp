#include "DisplayWin32.h"
#include "Game.h"

DisplayWin32::DisplayWin32(Game* game,
    const std::wstring& windowName,
    int width,
    int height)
    : game(game),
    windowName(windowName),
    clientWidth(width),
    clientHeight(height),
    hInstance(GetModuleHandle(nullptr)),
    hWnd(nullptr)
{
}

DisplayWin32::~DisplayWin32()
{
    if (hWnd)
    {
        DestroyWindow(hWnd);
    }
}

bool DisplayWin32::Initialize()
{
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = DisplayWin32::WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(LONG_PTR);
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = windowName.c_str();

    if (!RegisterClassEx(&wc))
        return false;

    RECT rect{ 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    int posX = (GetSystemMetrics(SM_CXSCREEN) - clientWidth) / 2;
    int posY = (GetSystemMetrics(SM_CYSCREEN) - clientHeight) / 2;

    hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        windowName.c_str(),
        windowName.c_str(),
        WS_OVERLAPPEDWINDOW,
        posX,
        posY,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        this);

    if (!hWnd)
        return false;

    return true;
}

void DisplayWin32::Show()
{
    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
}

LRESULT CALLBACK DisplayWin32::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    DisplayWin32* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<DisplayWin32*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    else
    {
        window = reinterpret_cast<DisplayWin32*>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window)
        return window->MessageHandler(hwnd, msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT DisplayWin32::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        if (game)
            game->OnKeyDown(static_cast<unsigned int>(wParam));
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
