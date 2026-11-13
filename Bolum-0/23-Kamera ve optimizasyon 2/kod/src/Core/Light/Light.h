#pragma once

#include <algorithm>
#include <glm/glm/glm.hpp>

#include "../TemelTanimlar/Defs.h"

class Light
{
public:
	Light(glm::vec3 direction);
	~Light();

	Color_t applyLighting(Color_t color, float intensity);

	glm::vec3 m_direction;
private:

};
