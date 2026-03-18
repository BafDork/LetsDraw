#pragma once

#include <unordered_map>

class GameApp;

class InputDevice
{
public:
    explicit InputDevice(GameApp* gameApp);

    void OnKeyDown(unsigned int vkey, bool pressed);
    void OnMouseMove(int dx, int dy);

    bool IsKeyDown(unsigned int key) const;
    bool IsKeyPressed(unsigned int key) const;
    bool IsKeyReleased(unsigned int key) const;

    std::pair<int, int> GetMousePosition() const { return { mMouseX, mMouseY }; }
    int GetMouseDeltaX() const { return mMouseDeltaX; }
    int GetMouseDeltaY() const { return mMouseDeltaY; }

    void EndFrame();

private:
    void RegisterRawInput();

private:
    GameApp* mGameApp;

    std::unordered_map<unsigned int, bool> mKeys;
    std::unordered_map<unsigned int, bool> mPrevKeys;

    int mMouseX = 0;
    int mMouseY = 0;

    int mMouseDeltaX = 0;
    int mMouseDeltaY = 0;
};
