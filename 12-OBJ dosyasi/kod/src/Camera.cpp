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
        vec.x * FOV_factor,
        vec.y * FOV_factor
    };
}

Vector2 Camera::projectPerspective(Vector3 vec)
{
    return Vector2
    {
        (vec.x * FOV_factor) / vec.z,
        (vec.y * FOV_factor) / vec.z
    };
}