#include "DisplayWin32.h"
#include "Game\Game.h"
#include "InputDevice.h"

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
		DestroyWindow(hWnd);
}

bool DisplayWin32::Initialize()
{
	WNDCLASSEX windowClass{};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = DisplayWin32::WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = sizeof(LONG_PTR);
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	windowClass.hIconSm = windowClass.hIcon;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = windowName.c_str();

	if (!RegisterClassEx(&windowClass))
	{
		return false;
	}

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

	return hWnd != nullptr;
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
		window = reinterpret_cast<DisplayWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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

	case WM_INPUT:
	{
		UINT size = 0;
		GetRawInputData((HRAWINPUT) lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

		BYTE* buffer = new BYTE[size];
		UINT realSize = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));

		if (realSize == size)
		{
			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);

			auto* input = game->GetInput();

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				bool pressed = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
				input->OnKeyDown(raw->data.keyboard.VKey, pressed);
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				input->OnMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			}
		}

		delete[] buffer;
		return 0;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}