#pragma once

#include <numbers>

#include "math/Vector2.h"
#include "math/Vector3.h"

class Camera
{
public:
	Camera();
	~Camera();

    Vector2 projectOrtho(Vector3 vec);
    Vector2 projectPerspective(Vector3 vec);        

    Vector3 eye = { 0,0,-5 };
    Vector3 target = { 0,0,0 };
    Vector3 up = { 0,1,0 };
    Vector3 forward = { 0, 0, 1 };

    Vector3 position = { 0,0,-5 };
    Vector3 direction = { 0,0,1 };
    Vector3 velocity = { 0,0,0 };
    float yawAngle = 0.0f;

    //60deg
    float FOV = std::numbers::pi / 3;
    float aspect = 600.0f / 800.0f;

    float zfar = 100.0f;
    float znear = 0.01f;
    float speed = 0.1f;
private:

};