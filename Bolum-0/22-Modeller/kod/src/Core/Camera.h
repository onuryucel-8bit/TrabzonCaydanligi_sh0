#pragma once

#include <iostream>
#include <assert.h>
#include <vector>

#include <spdlog/spdlog.h>

#include <glm/glm/glm.hpp>

class Camera
{
public:
	Camera(float fovY_, float znear_, float zfar_, float aspectx);
	~Camera();
		
	//void createPolygon(Vector3 a, Vector3 b, Vector3 c);
	void clip();
	bool clipProj(glm::vec4 vec);
	bool isInsidePlane(glm::vec4 vec);

	float m_znear = 0.5f;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_forwardVel;
	float yav;
	
private:		

};

