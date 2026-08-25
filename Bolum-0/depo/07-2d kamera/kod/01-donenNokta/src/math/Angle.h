#pragma once

#include <numbers>

inline float degToRad(float deg)
{
    return deg * std::numbers::pi / 180.0;
}

inline float radToDeg(float rad)
{
    return rad * 180.0 / std::numbers::pi;
}