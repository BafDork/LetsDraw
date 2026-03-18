#pragma once

#include "Engine\GameApp.h"

class PlanetariumGame : public GameApp
{
public:
    PlanetariumGame(int width, int height);

protected:
    void OnCreateGame() override;
    void OnUpdate(float delta) override;

private:
    float moveSpeed = 5.0f;
};
