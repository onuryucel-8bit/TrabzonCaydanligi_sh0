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

    Vector4 multiply(Vector4 vec);	

	float mat[4][4];

private:

};

