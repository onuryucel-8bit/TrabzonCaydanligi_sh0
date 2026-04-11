#include "Vector2.h"

Vector2::Vector2(float x_, float y_)
{
	x = x_;
	y = y_;
}

Vector2::~Vector2()
{
}

Vector2 Vector2::operator+ (const Vector2& vec)
{
	return Vector2(x + vec.x, y + vec.y);
}

Vector2 Vector2::operator-(const Vector2& vec)
{
	return Vector2(0,0);
}

void Vector2::operator*(float b)
{
}

void Vector2::operator/(float s)
{
}

void Vector2::normalize()
{
	float length = length();

	x /= length;
	y /= length;
}

void Vector2::dot()
{
}

void Vector2::cross()
{
}

float Vector2::length()
{
	return std::sqrt(x * x + y * y);
}

float Vector2::length(const Vector2& vec)
{
	return 0.0f;
}
