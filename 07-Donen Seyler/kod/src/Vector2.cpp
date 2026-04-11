#include "Vector2.h"

Vector2::Vector2(float x_, float y_)
{
	x = x_;
	y = y_;
}

Vector2::~Vector2()
{
}

Vector2 Vector2::operator+ (const Vector2& b)
{
	return Vector2(x + b.x, y + b.y);
}

Vector2 Vector2::operator-(const Vector2& b)
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
}

void Vector2::dot()
{
}

void Vector2::cross()
{
}

void Vector2::length()
{
}
