#pragma once

#include <glm/glm/glm.hpp>

struct Vertex
{
	glm::vec4 pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec2 uv = glm::vec2(0);
	glm::vec3 normal = glm::vec3(0);
};