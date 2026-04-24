#include "Vector2.h"

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x_, float y_)
{
	x = x_;
	y = y_;
}

Vector2::~Vector2()
{
}

Vector2 Vector2::operator+(const Vector2& vec)
{
	return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator-(const Vector2& vec)
{
	return Vector2(x - vec.x, y - vec.y);
}

Vector2 Vector2::operator*(float s)
{
	return Vector2(x * s, y * s);
}

Vector2 Vector2::operator/(float s)
{
	return Vector2(x / s, y / s);
}

void Vector2::normalize()
{
	float len = length();
	
	if (len > 0.0f)
	{
		x /= len;
		y /= len;
	}
}

float Vector2::dot(const Vector2& vec)
{	
	return x * vec.x + y * vec.y;
}


float Vector2::length()
{
	return std::sqrt(x * x + y * y);
}

