#pragma once

#include <assert.h>

#include "math/Vector3.h"
#include "math/Vector2.h"

enum class ProjectMod
{
	Ortho,
	Perspective
};

class Camera
{
public:
	Camera();
	~Camera();
	
	Vector2 project(Vector3 vec);

	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_forwardVel;
	float yav;
	
	ProjectMod m_projectMod = ProjectMod::Perspective;

private:
	
	Vector2 projectPerspective(Vector3 vec);
	Vector2 projectOrtho(Vector3 vec);
	
};

