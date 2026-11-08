#include "Camera.h"

Camera::Camera(float fovY_, float znear_, float zfar_, float aspectx)
{
	m_position = glm::vec3(0.0f, 0.0f, -4.0f );
	m_direction = glm::vec3(0, 0, 1);
	m_forwardVel = glm::vec3( 0,0,0 );
	m_yav = 0;
	m_znear = znear_;
	m_pitch = 0;
}

Camera::~Camera()
{
}

//void Camera::createPolygon(Vector3 a, Vector3 b, Vector3 c)
//{	
//	m_polygonVertices.clear();
//
//	m_polygonVertices.push_back(a);
//	m_polygonVertices.push_back(b);
//	m_polygonVertices.push_back(c);
//}

bool Camera::clipProj(glm::vec4 vec)
{
	float a = vec.w - m_znear;
	//std::cout << m_znear <<"\n";
	//spdlog::info("w={} znear={} d={}", vec.w, m_znear, a);
	//spdlog::info("vec.x {}, vec.y {}, vec.z {}", vec.x, vec.y, vec.z);
	if (a >= 0 && isInsidePlane(vec))
	{
		//spdlog::info("icerde");
		return false;
	}
	
	//spdlog::info("arkada");
	return true;
	
}

bool Camera::isInsidePlane(glm::vec4 vec)
{
	//spdlog::info("vec.x {}, vec.y {}, vec.z {}", vec.x, vec.y, vec.z);

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
