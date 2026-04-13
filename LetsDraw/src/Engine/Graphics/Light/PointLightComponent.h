#pragma once

#include "BaseLightComponent.h"

class PointLightComponent : public BaseLightComponent
{
public:
    PointLightComponent(GameApp* game) :
        BaseLightComponent(game) {}

    void SetRadius(float radius) { mRadius = radius; }
    float GetRadius() const { return mRadius; }

private:
    float mRadius = 10.0f;
};
