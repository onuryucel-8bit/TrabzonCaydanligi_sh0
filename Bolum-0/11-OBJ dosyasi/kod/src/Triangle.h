#pragma once

#include "math/Vector2.h"

struct Triangle
{
    Vector2 points[3];
};

struct Face
{
    int a, b, c;
};
