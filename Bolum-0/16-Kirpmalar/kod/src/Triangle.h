#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"

#include "Defs.h"

struct Triangle
{
    Vector4 points[3];
    Color_t color;
    float depthTestValue;
};

struct Face
{
    int a, b, c;
};
