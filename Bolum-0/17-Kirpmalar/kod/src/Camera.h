#pragma once

#include <numbers>
#include <vector>
#include <iostream>

#include "math/Vector2.h"
#include "math/Vector3.h"

enum FrustumPlane
{
    Left,
    Right,
    Top,
    Bottom,
    Near,
    Far

};

struct Plane
{
    Vector3 point;
    Vector3 normal;
};

struct Polygon
{
    std::vector<Vector3> vertices;    
};

class Camera
{
public:
    Camera();
	Camera(float FOV_, float znear_, float zfar_);
	~Camera();

    void init(float FOVx_, float FOVy_, float znear_, float zfar_, float aspectx_, float aspecty);

    //Vector2 projectOrtho(Vector3 vec);
    //Vector2 projectPerspective(Vector3 vec);        

    Polygon createPolygon(Vector3 a, Vector3 b, Vector3 c);
    void clip(Polygon& polygon);
    void clipPlane(Polygon& polygon, FrustumPlane plane);

    Vector3 eye = { 0,0,-5 };
    Vector3 target = { 0,0,0 };
    Vector3 up = { 0,1,0 };
    Vector3 forward = { 0, 0, 1 };

    Vector3 position = { 0,1.5,-5 };
    Vector3 direction = { 0,0,1 };
    Vector3 velocity = { 0,0,0 };

    Plane frustumPlanes[6];

    float yawAngle = 0.0f;

    //60deg
   
    float FOVx;
    float FOVy;
  
    float aspectx;
    float aspecty;

    float zfar = 100.0f;
    float znear = 0.01f;
    float speed = 0.1f;
private:

};
