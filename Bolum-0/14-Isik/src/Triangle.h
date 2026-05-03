#pragma once

#include "math/Vector2.h"

#include "Defs.h"

struct Triangle
{
    Vector2 points[3];
    Color_t color;
};

struct Face
{
    int a, b, c;
};
