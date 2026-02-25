#pragma once

#include <unordered_map>
#include <utility>

class Game;

class InputDevice
{
public:
    explicit InputDevice(Game* game);

    void AddPressedKey(unsigned int key);
    void RemovePressedKey(unsigned int key);
    bool IsKeyDown(unsigned int key) const;

    void OnMouseMove(int x, int y);
    std::pair<int, int> GetMousePosition() const;

private:
    Game* game;

    std::unordered_map<unsigned int, bool> keys;
    int mouseX = 0;
    int mouseY = 0;
};
