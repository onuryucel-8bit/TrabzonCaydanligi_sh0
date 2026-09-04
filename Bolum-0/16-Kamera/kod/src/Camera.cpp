#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{

}

Vector2 Camera::projectOrtho(Vector3 vec)
{
    return Vector2
    {
        vec.x * FOV,
        vec.y * FOV
    };
}

Vector2 Camera::projectPerspective(Vector3 vec)
{
    float factor = 1.0f / tan(FOV / 2.0f);

    return Vector2
    {
        vec.x * factor / vec.z,
        vec.y * factor / vec.z
    };
}