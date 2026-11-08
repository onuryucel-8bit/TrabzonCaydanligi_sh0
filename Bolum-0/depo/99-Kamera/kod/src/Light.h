#pragma once

#include "math/Vector3.h"

#include "Defs.h"

class Light
{
public:
	Light(Vector3 direction);
	~Light();

	Color_t applyLighting(Color_t color, float intensity);

	Vector3 m_direction;
private:

};
