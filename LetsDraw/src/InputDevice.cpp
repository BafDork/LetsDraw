#include "InputDevice.h"
#include "Game.h"
#include "DisplayWin32.h"

InputDevice::InputDevice(Game* game)
    : game(game)
{
    RegisterRawInput();
}

InputDevice::~InputDevice() = default;

void InputDevice::RegisterRawInput()
{
    RAWINPUTDEVICE rid[2]{};

    // Keyboard
    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = game->GetDisplay()->GetHWND();

    // Mouse
    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x02;
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = game->GetDisplay()->GetHWND();

    RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));
}

void InputDevice::OnKeyDown(unsigned int vkey, bool pressed)
{
    keys[vkey] = pressed;
}

bool InputDevice::IsKeyDown(unsigned int key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second;
}

void InputDevice::OnMouseMove(int dx, int dy)
{
    mouseDeltaX += dx;
    mouseDeltaY += dy;

    mouseX += dx;
    mouseY += dy;
}

std::pair<int, int> InputDevice::GetMousePosition() const
{
    return { mouseX, mouseY };
}

std::pair<int, int> InputDevice::GetMouseDelta() const
{
    return { mouseDeltaX, mouseDeltaY };
}

void InputDevice::EndFrame()
{
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}