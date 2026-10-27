#include "Camera.h"

Camera::Camera()
{
	m_position = Vector3{ 0.0f, 0.0f, -5.0f };
	m_direction = Vector3(0, 0, 1);
	m_forwardVel = { 0,0,0 };
	yav = 0;
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
	assert(vec.z != 0 && "SIFIRA BOLME HATASI VAR!!! OBJE KAMERANIN GOTUNE GIRDI");

	return Vector2
	{
		0,0
		//(vec.x * FOV_factor) / vec.z,
		//(vec.y * FOV_factor) / vec.z
	};	
}

Vector2 Camera::projectOrtho(Vector3 vec)
{
	//FOV_factor: gelen koordinatlar -1,1 gibi dar bir aralikta olucagi icin
	//bu degerleri ekrana uygun sekilde buyutmemiz gerekiyor
	return Vector2
	{
		0,0
		//vec.x * FOV_factor,
		//vec.y * FOV_factor
	};
}
