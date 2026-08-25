#pragma once

#include "Vector2.h"

class Matrix2x2
{
public:
	Matrix2x2();
	~Matrix2x2();

	static Matrix2x2 makeScaleMatrix(Vector2 scale);
	static Matrix2x2 makeRotateMatrix(float alfa);

	Vector2 operator*(Vector2 vec);

	float mat[2][2];
private:

};