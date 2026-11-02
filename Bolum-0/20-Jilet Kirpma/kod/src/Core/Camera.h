#pragma once

#include <iostream>
#include <assert.h>
#include <vector>

#include <spdlog/spdlog.h>

#include "math/Vector3.h"
#include "math/Vector2.h"

enum FrustumPlane
{
	Left,
	Right,
	Top,
	Bottom,
	Near,
	Far

};

struct Plane
{
	Vector3 point;
	Vector3 normal;
};

class Camera
{
public:
	Camera(float fovY_, float znear_, float zfar_, float aspectx);
	~Camera();
		
	void createPolygon(Vector3 a, Vector3 b, Vector3 c);
	void clip();
	bool clipProj(Vector4 vec);
	bool isInsidePlane(Vector4 vec);

	float m_znear = 0.5f;
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_forwardVel;
	float yav;
	
	Plane frustumPlanes[6];

	std::vector<Vector3> m_polygonVertices;

private:		
	void clipPlane(FrustumPlane planeType);
};

