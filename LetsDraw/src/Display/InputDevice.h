#pragma once

#include <unordered_map>

class Game;

class InputDevice
{
public:
    explicit InputDevice(Game* game);
    ~InputDevice() = default;

    void OnKeyDown(unsigned int vkey, bool pressed);
    void OnMouseMove(int dx, int dy);

    bool IsKeyDown(unsigned int key) const;

    std::pair<int, int> GetMousePosition() const { return { mouseX, mouseY }; }
    std::pair<int, int> GetMouseDelta() const { return { mouseDeltaX, mouseDeltaY }; }

    void EndFrame();

private:
    void RegisterRawInput();

private:
    Game* game;

    std::unordered_map<unsigned int, bool> keys;

    int mouseX = 0;
    int mouseY = 0;

    int mouseDeltaX = 0;
    int mouseDeltaY = 0;
};
