#include "Vector3.h"

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::operator+(const Vector3& vec)
{
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

Vector3 Vector3::operator-(const Vector3& vec)
{
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

Vector3 Vector3::operator*(float b)
{
	return Vector3(x * b, y * b, z * b);
}

Vector3 Vector3::operator/(float s)
{
	return Vector3(x / s, y / s, z / s);
}


float Vector3::length()
{
	return std::sqrt(x * x + y * y + z * z);
}


void Vector3::normalize()
{
	float len = length();
	if (len > 0.0f)
	{
		x /= len;
		y /= len;
		z /= len;
	}
}

Vector3 Vector3::cross(const Vector3& vec)
{
	Vector3 res;

	res.x = y * vec.z - z * vec.y;
	res.y = z * vec.x - x * vec.z;
	res.z = x * vec.y - y * vec.x;

	return res;
}

float Vector3::dot(const Vector3& vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}