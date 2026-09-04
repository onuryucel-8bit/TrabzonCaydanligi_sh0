#pragma once

#include "Vector2.h"

class Matrix3x3
{
public:
	Matrix3x3();
	~Matrix3x3();

    static Matrix3x3 makeTranslationMatrix(Vector2 translation);

    Vector2 operator*(Vector2 vec);

	float mat[3][3];
private:

};

