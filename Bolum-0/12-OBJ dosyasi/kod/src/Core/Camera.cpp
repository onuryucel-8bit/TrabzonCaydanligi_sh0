#include "Camera.h"

Camera::Camera()
{
	m_position = Vector3{ 0.0f, 0.0f, -3.0f };
}

Camera::~Camera()
{
}

Vector2 Camera::project(Vector3 vec)
{
	switch (m_projectMod)
	{
	case ProjectMod::Perspective:
		return projectPerspective(vec);
		
	case ProjectMod::Ortho:
		return projectOrtho(vec);		
	}
}

Vector2 Camera::projectPerspective(Vector3 vec)
{	
	return Vector2
	{
		(vec.x * FOV_factor) / vec.z,
		(vec.y * FOV_factor) / vec.z
	};	
}

Vector2 Camera::projectOrtho(Vector3 vec)
{
	//FOV_factor: gelen koordinatlar -1,1 gibi dar bir aralikta olucagi icin
	//bu degerleri ekrana uygun sekilde buyutmemiz gerekiyor
	return Vector2
	{
		vec.x * FOV_factor,
		vec.y * FOV_factor
	};
}
