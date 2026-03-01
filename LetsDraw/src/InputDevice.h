#pragma once

#include <unordered_map>
#include <utility>
#include <Windows.h>

class Game;

class InputDevice
{
public:
    explicit InputDevice(Game* game);
    ~InputDevice();

    // вызывается из WndProc
    void OnKeyDown(unsigned int vkey, bool pressed);
    void OnMouseMove(int dx, int dy);

    // polling API (используется игрой)
    bool IsKeyDown(unsigned int key) const;

    std::pair<int, int> GetMousePosition() const;
    std::pair<int, int> GetMouseDelta() const;

    // вызывается каждый кадр из Game::Update
    void EndFrame();

private:
    void RegisterRawInput();

private:
    Game* game = nullptr;

    std::unordered_map<unsigned int, bool> keys;

    int mouseX = 0;
    int mouseY = 0;

    int mouseDeltaX = 0;
    int mouseDeltaY = 0;
};
