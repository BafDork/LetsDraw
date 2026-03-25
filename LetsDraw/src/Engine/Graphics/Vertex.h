#pragma once

#include <SimpleMath.h>

using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector2;

struct Vertex
{
    Vector4 position;
    Vector4 color;
    Vector2 uv;
};