#pragma once

#include <vector>

#include "math/Vector3.h"
#include "math/Vector2.h"
#include "Defs.h"

struct Triangle
{
    Vector2 points[3];    
};

struct Face
{
    int a, b, c;
};

void loadCube(std::vector<Vector3>& modelPoints, std::vector<Face>& meshFaces);

void loadCubeCloud(std::vector<Vector3>& modelPoints, Vector3 position, float inc);

void loadTriangle(std::vector<Vector3>& modelPoints, std::vector<Face>& meshFaces);
