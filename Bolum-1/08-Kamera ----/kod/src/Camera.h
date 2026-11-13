#pragma once

#include <glm/glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();
		
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_forwardVel;
	float m_yav;
	float m_pitch;

private:

};
