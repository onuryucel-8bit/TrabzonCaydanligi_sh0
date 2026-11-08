#pragma once

#include <glm/glm/glm.hpp>

#include "Defs.h"

struct Triangle
{
    glm::vec4 points[3];
    glm::vec2 texcoords[3];
    Color_t color;    
};