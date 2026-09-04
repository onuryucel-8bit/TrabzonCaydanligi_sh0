#pragma once

#include "Texture.h"

struct Face
{
    int a, b, c;

    Texture a_uv;
    Texture b_uv;
    Texture c_uv;
};