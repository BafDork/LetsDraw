#include "InputDevice.h"
#include "Game.h"

InputDevice::InputDevice(Game* game)
    : game(game)
{
}

void InputDevice::AddPressedKey(unsigned int key)
{
    keys[key] = true;
}

void InputDevice::RemovePressedKey(unsigned int key)
{
    keys[key] = false;
}

bool InputDevice::IsKeyDown(unsigned int key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second;
}

void InputDevice::OnMouseMove(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

std::pair<int, int> InputDevice::GetMousePosition() const
{
    return { mouseX, mouseY };
}