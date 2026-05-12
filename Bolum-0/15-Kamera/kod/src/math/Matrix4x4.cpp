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

Matrix4x4 Matrix4x4::makePerspectiveMatrix(float fov, float aspect, float znear, float zfar)
{
    Matrix4x4 matrix;

    float fovtan = 1.0f / tan(fov / 2);

    matrix.mat[0][0] = fovtan * aspect;
    matrix.mat[1][1] = fovtan;
    matrix.mat[2][2] = zfar / (zfar - znear);
    matrix.mat[2][3] = (-zfar * znear) / (zfar - znear);
    matrix.mat[3][2] = 1.0f;

    return matrix;
}

Matrix4x4 Matrix4x4::lookAt(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 z = target - eye;
    z.normalize();

    Vector3 x = up.cross(z);       
    x.normalize();

    Vector3 y = z.cross(x);
        
    Matrix4x4 m;

    m.mat[0][0] = x.x; m.mat[0][1] = x.y; m.mat[0][2] = x.z; m.mat[0][3] = -x.dot(eye);
    m.mat[1][0] = y.x; m.mat[1][1] = y.y; m.mat[1][2] = y.z; m.mat[1][3] = -y.dot(eye);
    m.mat[2][0] = z.x; m.mat[2][1] = z.y; m.mat[2][2] = z.z; m.mat[2][3] = -z.dot(eye);
    m.mat[3][0] = 0;   m.mat[3][1] = 0;   m.mat[3][2] = 0;   m.mat[3][3] = 1;

    return m;      
}

Vector4 Matrix4x4::operator*(Vector4 vec)
{
    Vector4 result;

    result.x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w;
    result.y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w;
    result.z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w;
    result.w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w;

    return result;
}

Vector4 Matrix4x4::multiplyProject(Vector4 vec)
{
    Vector4 result = (*this) * vec;

    if (result.w > 0.0f)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}


