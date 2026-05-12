#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"

class Camera
{
public:
	Camera();
	~Camera();

    Vector2 projectOrtho(Vector3 vec);
    Vector2 projectPerspective(Vector3 vec);
    
    float FOV_factor = 300;
    Vector3 position = { 0.0f, 0.0f, -3.0f };
private:

};