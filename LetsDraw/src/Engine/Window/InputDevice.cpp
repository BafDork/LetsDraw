#include "DisplayWin32.h"
#include "Engine\GameApp.h"
#include "InputDevice.h"

InputDevice::InputDevice(GameApp* gameApp)
    : mGameApp(gameApp)
{
    RegisterRawInput();
}

void InputDevice::RegisterRawInput()
{
    RAWINPUTDEVICE rid[2] {};

    rid[0].usUsagePage = 0x01;
    rid[0].usUsage = 0x06;
    rid[0].dwFlags = 0;
    rid[0].hwndTarget = mGameApp->GetDisplay()->GetHWND();

    rid[1].usUsagePage = 0x01;
    rid[1].usUsage = 0x02;
    rid[1].dwFlags = 0;
    rid[1].hwndTarget = mGameApp->GetDisplay()->GetHWND();

    RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));
}

void InputDevice::OnKeyDown(unsigned int vkey, bool pressed)
{
    mKeys[vkey] = pressed;
}

bool InputDevice::IsKeyDown(unsigned int key) const
{
    auto iterator = mKeys.find(key);
    return iterator != mKeys.end() && iterator->second;
}

bool InputDevice::IsKeyPressed(unsigned int key) const
{
    bool current = false;
    bool previous = false;

    auto itCur = mKeys.find(key);
    if (itCur != mKeys.end())
        current = itCur->second;

    auto itPrev = mPrevKeys.find(key);
    if (itPrev != mPrevKeys.end())
        previous = itPrev->second;

    return current && !previous;
}

bool InputDevice::IsKeyReleased(unsigned int key) const
{
    bool current = false;
    bool previous = false;

    auto itCur = mKeys.find(key);
    if (itCur != mKeys.end())
        current = itCur->second;

    auto itPrev = mPrevKeys.find(key);
    if (itPrev != mPrevKeys.end())
        previous = itPrev->second;

    return !current && previous;
}

void InputDevice::OnMouseMove(int deltaX, int deltaY)
{
    mMouseDeltaX += deltaX;
    mMouseDeltaY += deltaY;

    mMouseX += deltaX;
    mMouseY += deltaY;
}

void InputDevice::EndFrame()
{
    mPrevKeys = mKeys;

    mMouseDeltaX = 0;
    mMouseDeltaY = 0;
}