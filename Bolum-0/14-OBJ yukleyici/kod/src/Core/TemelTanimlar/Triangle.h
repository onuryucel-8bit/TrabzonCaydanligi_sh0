#pragma once

#include "math/Vector2.h"
#include "Defs.h"
#include "Texture.h"

struct Triangle
{
    Vector2 points[3];
    Texture texcoords[3];
    Color_t color;
    float depthTestValue;
};