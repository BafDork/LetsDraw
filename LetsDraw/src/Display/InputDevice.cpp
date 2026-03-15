#include "DisplayWin32.h"
#include "Game\Game.h"
#include "InputDevice.h"

InputDevice::InputDevice(Game* game)
    : game(game)
{
    RegisterRawInput();
}

void InputDevice::RegisterRawInput()
{
    RAWINPUTDEVICE rid[2]{};

    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = game->GetDisplay()->GetHWND();

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
    auto iterator = keys.find(key);
    return iterator != keys.end() && iterator->second;
}

void InputDevice::OnMouseMove(int dx, int dy)
{
    mouseDeltaX += dx;
    mouseDeltaY += dy;

    mouseX += dx;
    mouseY += dy;
}

void InputDevice::EndFrame()
{
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}