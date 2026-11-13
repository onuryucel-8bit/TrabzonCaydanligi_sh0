#pragma once

#include <cmath>

#include "Vector4.h"

class Vector4;

class Vector3
{
public:
	Vector3();
	Vector3(float x_, float y_, float z_);
	~Vector3();

	Vector4 toVec4();

	[[nodiscard]] Vector3 operator+ (const Vector3& vec);
	[[nodiscard]] Vector3 operator- (const Vector3& vec);
	[[nodiscard]] Vector3 operator* (float s);
	[[nodiscard]] Vector3 operator/ (float s);

	Vector3 rotateX(float alfa);
	Vector3 rotateY(float alfa);
	Vector3 rotateZ(float alfa);

	void normalize();
	float dot(const Vector3& vec);
	Vector3 cross(const Vector3& vec);

	float length();

	float x, y, z;

private:

};

