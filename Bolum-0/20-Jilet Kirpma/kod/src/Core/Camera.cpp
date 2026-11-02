#include "Camera.h"

Camera::Camera(float fovY_, float znear_, float zfar_, float aspectx)
{
	m_position = Vector3{ 0.0f, 0.0f, -5.0f };
	m_direction = Vector3(0, 0, 1);
	m_forwardVel = { 0,0,0 };
	yav = 0;
	//m_znear = znear_;
	
	float fovX = 2.0f * atan(tan(fovY_ / 2) * aspectx);   // yatay FOV

	float cosV = cos(fovY_ / 2);
	float sinV = sin(fovY_ / 2);
	float cosH = cos(fovX / 2);
	float sinH = sin(fovX / 2);
	//std::cout << "cosh" << cosH << " sinh" << sinH << "\n";
	spdlog::info("cosh {} , sinh {} ", cosH, sinH);

	frustumPlanes[FrustumPlane::Left].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Left].normal = { cosH, 0, sinH };

	frustumPlanes[FrustumPlane::Right].point = { 0, 0, 0 };
	frustumPlanes[FrustumPlane::Right].normal = { -cosH, 0, sinH };

	frustumPlanes[FrustumPlane::Top].point = { 0, 0, 0 };
	//frustumPlanes[FrustumPlane::Top].normal = { 0, -cosV, sinV };
	frustumPlanes[FrustumPlane::Top].normal = { 0,  cosV, sinV };

	frustumPlanes[FrustumPlane::Bottom].point = { 0, 0, 0 };
	//frustumPlanes[FrustumPlane::Bottom].normal = { 0,  cosV, sinV };
	frustumPlanes[FrustumPlane::Bottom].normal = { 0, -cosV, sinV };
	
	

	frustumPlanes[FrustumPlane::Near].point = { 0, 0, znear_ };
	frustumPlanes[FrustumPlane::Near].normal = { 0, 0, 1 };

	frustumPlanes[FrustumPlane::Far].point = { 0, 0, zfar_ };
	frustumPlanes[FrustumPlane::Far].normal = { 0, 0, -1 };
}

Camera::~Camera()
{
}

void Camera::createPolygon(Vector3 a, Vector3 b, Vector3 c)
{	
	m_polygonVertices.clear();

	m_polygonVertices.push_back(a);
	m_polygonVertices.push_back(b);
	m_polygonVertices.push_back(c);
}

void Camera::clip()
{
	clipPlane(FrustumPlane::Near);
	//std::cout << "Left: " << m_polygonVertices.size() << '\n';

	//clipPlane(FrustumPlane::Right);
	//std::cout << "Right: " << m_polygonVertices.size() << '\n';

	//clipPlane(FrustumPlane::Top);
	//std::cout << "Top: " << m_polygonVertices.size() << '\n';

	//clipPlane(FrustumPlane::Bottom);
	//std::cout << "Bottom: " << m_polygonVertices.size() << '\n';

	//clipPlane(FrustumPlane::Near);
	//std::cout << "Near: " << m_polygonVertices.size() << '\n';

	//clipPlane(FrustumPlane::Far);
	//std::cout << "Far: " << m_polygonVertices.size() << '\n';
}

bool Camera::clipProj(Vector4 vec)
{
	float a = vec.w - m_znear;
	//std::cout << m_znear <<"\n";
	//spdlog::info("w={} znear={} d={}", vec.w, m_znear, a);

	if (a >= 0 && isInsidePlane(vec))
	{
		//spdlog::info("icerde");
		return false;
	}
	
	//spdlog::info("arkada");
	return true;
	
}

bool Camera::isInsidePlane(Vector4 vec)
{
	if (vec.x >= -vec.w && vec.x <= vec.w &&
		vec.y >= -vec.w && vec.y <= vec.w &&
		vec.z >= -vec.w && vec.z <= vec.w)
	{
		//spdlog::info("icerde");
		return true;
	}
	
	return false;		
	//spdlog::info("pencerenin disinda kaldi");
	
}

void Camera::clipPlane(FrustumPlane planeType)
{
	if (m_polygonVertices.empty())
	{
		return;
	}

	Vector3 planePoint = frustumPlanes[planeType].point;
	Vector3 planeNormal = frustumPlanes[planeType].normal;


	std::vector<Vector3> outputVertices;
	outputVertices.reserve(m_polygonVertices.size() + 2);

	Vector3 currentVertex = m_polygonVertices[0];
	Vector3 previousVertex = m_polygonVertices.back();

	float previousDot = (previousVertex - planePoint).dot(planeNormal);

	for (size_t i = 0; i < m_polygonVertices.size(); i++)
	{
		 currentVertex = m_polygonVertices[i];


		float currentDot = (currentVertex - planePoint).dot(planeNormal);

		if (currentDot * previousDot < 0)
		{
			float t = previousDot / (previousDot - currentDot);
			outputVertices.push_back(previousVertex + (currentVertex - previousVertex) * t);
		}

		if (currentDot > 0)
		{
			outputVertices.push_back(currentVertex);
		}

		previousVertex = currentVertex;
		previousDot = currentDot;
	}

	//std::cout << outputVertices.size() << "\n";

	m_polygonVertices = std::move(outputVertices);
}

