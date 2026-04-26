#pragma once

#include "math/Vector2.h"

struct Triangle
{
    Vector2 points[3];
    float averageDepth = 0;
};

struct Face
{
    int a, b, c;
};
