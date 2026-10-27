#pragma once

#include "Vector4.h"
#include "Vector3.h"

class Matrix4x4
{
public:
	Matrix4x4();
	~Matrix4x4();

	static Matrix4x4 makeScaleMatrix(Vector3 scale);
	static Matrix4x4 makeTranslationMatrix(Vector3 translate);
	static Matrix4x4 makeRotateXMatrix(float alfa);
	static Matrix4x4 makeRotateYMatrix(float alfa);
	static Matrix4x4 makeRotateZMatrix(float alfa);

	static Matrix4x4 makePerspectiveMatrix(float fov, float aspect, float znear, float zfar);
	static Matrix4x4 makeOrthographicMatrix(float left,
		float right,
		float bottom,
		float top,
		float znear,
		float zfar);

	static Matrix4x4 lookAt(Vector3 eye, Vector3 target, Vector3 up);
	static Vector4 project(Matrix4x4 mat, Vector4 vec);

	Vector4 operator*(Vector4 vec);
	Matrix4x4  operator*(Matrix4x4 b);  

	float mat[4][4];

private:

};

