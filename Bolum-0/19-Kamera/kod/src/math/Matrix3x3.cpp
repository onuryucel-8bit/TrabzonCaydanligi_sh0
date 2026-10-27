#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
{
    /*
      1   0   0
      0   1   0
      0   0   1
   */
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat[i][j] = 0.0f;
        }
    }

    mat[0][0] = 1;
    mat[1][1] = 1;
    mat[2][2] = 1;
}

Matrix3x3::~Matrix3x3()
{
}

Matrix3x3 Matrix3x3::makeTranslationMatrix(Vector2 translation)
{
    /*
        1   0   tx
        0   1   ty
        0   0   1
    */

    Matrix3x3 matrix;

    matrix.mat[0][2] = translation.x;
    matrix.mat[1][2] = translation.y;

    return matrix;
}

Vector2 Matrix3x3::operator*(Vector2 vec)
{
    //w = 1.0f
    Vector2 result;

    result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * 1.0f;
    result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * 1.0f;

    return result;
}
