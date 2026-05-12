#include "Camera.h"

Camera::Camera()
{
}

//TODO offff kurucuyu gormuyor
Camera::Camera(float FOV_, float znear_, float zfar_)
{
    /*

-------------
rightFrustrum
-------------
nx -cos(fov/2)
ny 0
nz sin(fov/2)

-------------
leftFrustrum
-------------
nx cos(fov/2)
ny 0
nz sin(fov/2)

-------------
topFrustrum
-------------
nx 0
ny -cos(fov/2)
nz  sin(fov/2)

-------------
bottomFrustrum
-------------
nx 0
ny  cos(fov/2)
nz  sin(fov/2)

-------------
znear
-------------
p(0,0,znear)

nx 0
ny 0
nz 1

-------------
zfar
-------------
p(0,0,zfar)

nx 0
ny 0
nz -1
*/

	znear = znear_;
	zfar = zfar_;
	FOVy = FOV_;
	FOVx = 0;
	aspectx = 0;
	aspecty = 0;


	float cosHalfFov = cos(FOVy / 2);
	float sinHalfFov = sin(FOVy / 2);

	frustumPlanes[FrustumPlane::Left].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Left].normal.x = cosHalfFov;
	frustumPlanes[FrustumPlane::Left].normal.y = 0;
	frustumPlanes[FrustumPlane::Left].normal.z = sinHalfFov;
	
	frustumPlanes[FrustumPlane::Right].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Right].normal.x = -cosHalfFov;
	frustumPlanes[FrustumPlane::Right].normal.y = 0;
	frustumPlanes[FrustumPlane::Right].normal.z = sinHalfFov;
	
	frustumPlanes[FrustumPlane::Top].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Top].normal.x = 0;
	frustumPlanes[FrustumPlane::Top].normal.y = -cosHalfFov;
	frustumPlanes[FrustumPlane::Top].normal.z = sinHalfFov;
	
	frustumPlanes[FrustumPlane::Bottom].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Bottom].normal.x = 0;
	frustumPlanes[FrustumPlane::Bottom].normal.y = cosHalfFov;
	frustumPlanes[FrustumPlane::Bottom].normal.z = sinHalfFov;
	
	frustumPlanes[FrustumPlane::Near].point = { 0, 0, znear };
	frustumPlanes[FrustumPlane::Near].normal.x = 0;
	frustumPlanes[FrustumPlane::Near].normal.y = 0;
	frustumPlanes[FrustumPlane::Near].normal.z = 1;
	
	frustumPlanes[FrustumPlane::Far].point = { 0, 0, zfar };
	frustumPlanes[FrustumPlane::Far].normal.x = 0;
	frustumPlanes[FrustumPlane::Far].normal.y = 0;
	frustumPlanes[FrustumPlane::Far].normal.z = -1;

}

Camera::~Camera()
{

}

Polygon Camera::createPolygon(Vector3 a, Vector3 b, Vector3 c)
{
	Polygon polygon;
	
	polygon.vertices.push_back(a);
	polygon.vertices.push_back(b);
	polygon.vertices.push_back(c);


	return polygon;
}


//https://en.wikipedia.org/wiki/Sutherland-Hodgman_algorithm
//https://dl.acm.org/doi/epdf/10.1145/360767.360802 
//https://www.sunshine2k.de/coding/java/SutherlandHodgman/SutherlandHodgman.html?utm_source=chatgpt.com

void Camera::clipPlane(Polygon& polygon, FrustumPlane plane)
{	
	if (polygon.vertices.empty())
	{
		return;
	}

	Vector3 planePoint = frustumPlanes[plane].point;
	Vector3 planeNormal = frustumPlanes[plane].normal;

	
	std::vector<Vector3> outputVertices;
	outputVertices.reserve(polygon.vertices.size() + 2);
	 
	Vector3 previousVertex = polygon.vertices.back();

	float previousDot = (previousVertex - planePoint).dot(planeNormal);
	
	for (size_t i = 0; i < polygon.vertices.size(); i++)
	{
		Vector3 currentVertex = polygon.vertices[i];
		

		float currentDot = (currentVertex - planePoint).dot(planeNormal);

		if (currentDot * previousDot < 0)
		{									
			float t = previousDot / (previousDot - currentDot);
			outputVertices.push_back(previousVertex + (currentVertex - previousVertex) * t);
									
		}
		
		if(currentDot > 0)
		{
			outputVertices.push_back(polygon.vertices[i]);
		}

		previousVertex = currentVertex;
		previousDot = currentDot;
	}
					
	/*if (outputVertices.empty())
	{
		polygon.vertices.clear();
	}
	else
	{*/
		polygon.vertices = outputVertices;
	//}				
}

void Camera::clip(Polygon& polygon)
{
	clipPlane(polygon, FrustumPlane::Left);
	clipPlane(polygon, FrustumPlane::Right);
	clipPlane(polygon, FrustumPlane::Top);
	clipPlane(polygon, FrustumPlane::Bottom);
	clipPlane(polygon, FrustumPlane::Near);
	clipPlane(polygon, FrustumPlane::Far);
	//std::cout << "------polygon.vertices.size()-------\n";
	//std::cout << polygon.vertices.size() << "\n";
}


void Camera::init(float FOVx_, float FOVy_, float znear_, float zfar_, float aspectx_, float aspecty)
{
	znear = znear_;
	zfar = zfar_;
	FOVx = FOVx_;
	FOVy = FOVy_;

	float cosHalfFovX = cos(FOVx / 2);
	float sinHalfFovX = sin(FOVx / 2);

	float cosHalfFovY = cos(FOVy / 2);
	float sinHalfFovY = sin(FOVy / 2);

	frustumPlanes[FrustumPlane::Left].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Left].normal.x = sinHalfFovX + 0.5;
	frustumPlanes[FrustumPlane::Left].normal.y = 0;
	frustumPlanes[FrustumPlane::Left].normal.z = cosHalfFovX;

	frustumPlanes[FrustumPlane::Right].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Right].normal.x = -cosHalfFovX;
	frustumPlanes[FrustumPlane::Right].normal.y = 0;
	frustumPlanes[FrustumPlane::Right].normal.z = sinHalfFovX;

	frustumPlanes[FrustumPlane::Top].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Top].normal.x = 0;
	frustumPlanes[FrustumPlane::Top].normal.y = -cosHalfFovY;
	frustumPlanes[FrustumPlane::Top].normal.z = sinHalfFovY;

	frustumPlanes[FrustumPlane::Bottom].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Bottom].normal.x = 0;
	frustumPlanes[FrustumPlane::Bottom].normal.y = cosHalfFovY;
	frustumPlanes[FrustumPlane::Bottom].normal.z = sinHalfFovY;

	/*frustumPlanes[FrustumPlane::Left].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Left].normal.x = cosHalfFovX;
	frustumPlanes[FrustumPlane::Left].normal.y = 0;
	frustumPlanes[FrustumPlane::Left].normal.z = sinHalfFovX;

	frustumPlanes[FrustumPlane::Right].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Right].normal.x = -cosHalfFovX;
	frustumPlanes[FrustumPlane::Right].normal.y = 0;
	frustumPlanes[FrustumPlane::Right].normal.z = sinHalfFovX;

	frustumPlanes[FrustumPlane::Top].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Top].normal.x = 0;
	frustumPlanes[FrustumPlane::Top].normal.y = -cosHalfFovX;
	frustumPlanes[FrustumPlane::Top].normal.z = sinHalfFovX;

	frustumPlanes[FrustumPlane::Bottom].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Bottom].normal.x = 0;
	frustumPlanes[FrustumPlane::Bottom].normal.y = cosHalfFovX;
	frustumPlanes[FrustumPlane::Bottom].normal.z = sinHalfFovX;*/

	frustumPlanes[FrustumPlane::Near].point = { 0, 0, znear };
	frustumPlanes[FrustumPlane::Near].normal.x = 0;
	frustumPlanes[FrustumPlane::Near].normal.y = 0;
	frustumPlanes[FrustumPlane::Near].normal.z = 1;

	frustumPlanes[FrustumPlane::Far].point = { 0, 0, zfar };
	frustumPlanes[FrustumPlane::Far].normal.x = 0;
	frustumPlanes[FrustumPlane::Far].normal.y = 0;
	frustumPlanes[FrustumPlane::Far].normal.z = -1;
}

//Vector2 Camera::projectOrtho(Vector3 vec)
//{
//    return Vector2
//    {
//        vec.x * FOV,
//        vec.y * FOV
//    };
//}
//
//Vector2 Camera::projectPerspective(Vector3 vec)
//{
//    float factor = 1.0f / tan(FOV / 2.0f);
//
//    return Vector2
//    {
//        vec.x * factor / vec.z,
//        vec.y * factor / vec.z
//    };
//}