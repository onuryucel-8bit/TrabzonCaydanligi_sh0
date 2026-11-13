#include "Matrix2x2.h"
#include "Matrix2x2.h"
#include "Matrix2x2.h"

Matrix2x2::Matrix2x2()
{

	/*		
		1  0
		0  1		
	*/
	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[1][0] = 0;
	mat[1][1] = 1;
}

Matrix2x2::~Matrix2x2()
{
}

Matrix2x2 Matrix2x2::makeScaleMatrix(Vector2 scale)
{
	Matrix2x2 matrix;

	matrix.mat[0][0] = scale.x;
	matrix.mat[1][1] = scale.y;
	
	return matrix;
}


Matrix2x2 Matrix2x2::makeRotateMatrix(float alfa)
{
	/*
		cosa  -sina
		sina   cosa	
	*/
	Matrix2x2 matrix;

	matrix.mat[0][0] =  cos(alfa);
	matrix.mat[0][1] = -sin(alfa);
	matrix.mat[1][0] =  sin(alfa);
	matrix.mat[1][1] =  cos(alfa);

	return matrix;	
}

Vector2 Matrix2x2::operator*(Vector2 vec)
{
	Vector2 result;

	result.x = mat[0][0] * vec.x + mat[0][1] * vec.y;
	result.y = mat[1][0] * vec.x + mat[1][1] * vec.y;

	return result;
}
