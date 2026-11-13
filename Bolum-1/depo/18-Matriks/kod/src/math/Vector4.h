#pragma once

#include "Vector3.h"

class Vector3;

class Vector4
{
public:
	Vector4();
	Vector4(float x_, float y_, float z_, float w_ );
	~Vector4();

	Vector3 toVec3();

	float x, y, z, w;
private:

};

