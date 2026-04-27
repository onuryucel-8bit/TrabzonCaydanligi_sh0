#pragma once

#include "math/Vector2.h"
#include "Defs.h"


struct Triangle
{
    Vector2 points[3];
    float averageDepth = 0;
    Color_t color;
};
