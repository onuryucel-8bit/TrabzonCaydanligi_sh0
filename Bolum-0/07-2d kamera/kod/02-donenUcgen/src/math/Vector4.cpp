#include "Vector4.h"

Vector4::Vector4()
{
}

Vector4::Vector4(float x_, float y_, float z_, float w_)
{
	x = x_;
	y = y_;
	z = z_;
	w = w_;
}

Vector4::~Vector4()
{
}

Vector4 Vector4::subVec3(const Vector3& vec)
{
	return Vector4(x - vec.x, y - vec.y, z - vec.z, w);
}


Vector3 Vector4::toVec3()
{
	return Vector3(x, y, z);
}
