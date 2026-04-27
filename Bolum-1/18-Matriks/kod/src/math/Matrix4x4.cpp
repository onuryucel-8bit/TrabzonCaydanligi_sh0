#include "Matrix4x4.h"
#include "Matrix4x4.h"
#include "Matrix4x4.h"
#include "Matrix4x4.h"
#include "Matrix4x4.h"
#include "Matrix4x4.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
    /*
       1   0   0   0
       0   1   0   0
       0   0   1   0
       0   0   0   0
    */
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = 0.0f;
        }
    }
    
    mat[0][0] = 1;
    mat[1][1] = 1;
    mat[2][2] = 1;
    mat[3][3] = 1;
}

Matrix4x4::~Matrix4x4()
{
}

Matrix4x4 Matrix4x4::makeScaleMatrix(Vector3 scale)
{    
    /*
        sx  0   0   0
        0   sy  0   0
        0   0   sz  0
        0   0   0   0
    */
    Matrix4x4 matrix;

    matrix.mat[0][0] = scale.x;
    matrix.mat[1][1] = scale.y;
    matrix.mat[2][2] = scale.z;
    

    return matrix;
}

Matrix4x4 Matrix4x4::makeTranslationMatrix(Vector3 translate)
{
    /*
        1   0   0   tx
        0   1   0   ty
        0   0   1   tz
        0   0   0   1
    */
    Matrix4x4 matrix;

    matrix.mat[0][3] = translate.x;
    matrix.mat[1][3] = translate.y;
    matrix.mat[2][3] = translate.z;


    return matrix;
}

Matrix4x4 Matrix4x4::makeRotateXMatrix(float alfa)
{
    /*
        1   0       0     0
        0   cosa  -sina   0
        0   sina   cosa   0
        0   0       0     1
    */

    Matrix4x4 matrix;

    matrix.mat[1][1] = cos(alfa);
    matrix.mat[1][2] = -sin(alfa);
    matrix.mat[2][1] = sin(alfa);
    matrix.mat[2][2] = cos(alfa);

    return matrix;
}

Matrix4x4 Matrix4x4::makeRotateYMatrix(float alfa)
{
    /*
        cosa    0   sina  0
        0       0   0     0
        -sina   0   cosa  0
        0       0   0     1
    */

    Matrix4x4 matrix;

    matrix.mat[0][0] =  cos(alfa);
    matrix.mat[0][2] =  sin(alfa);
    matrix.mat[2][0] = -sin(alfa);
    matrix.mat[2][2] =  cos(alfa);

    return matrix;
}

Matrix4x4 Matrix4x4::makeRotateZMatrix(float alfa)
{
    /*
        cosa -sina   0   0
        sina  cosa   0   0
        0       0    1   0
        0       0    0   1
    */

    Matrix4x4 matrix;

    matrix.mat[0][0] = cos(alfa);
    matrix.mat[0][1] = -sin(alfa);
    matrix.mat[1][0] = sin(alfa);
    matrix.mat[1][1] = cos(alfa);

    return matrix;
}

Vector4 Matrix4x4::multiply(Vector4 vec)
{
    Vector4 result;

    result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w;
    result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w;
    result.z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w;
    result.w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w;

    return result;
}
