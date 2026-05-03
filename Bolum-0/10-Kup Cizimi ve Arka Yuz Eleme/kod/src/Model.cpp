#include "Model.h"

void loadCube(std::vector<Vector3>& modelPoints, std::vector<Face>& meshFaces)
{
    /*
            (-1,1,1) ----------- (1,1,1)
               /|                  /|
              / |                 / |
             /  |                /  |
    (-1,1,-1)-----------(1,1,-1)    |
            |   |               |   |
            |   |               |   |
            |   |               |   |
            |   |               |   |
            |  (-1,-1,1)--------|---(1,-1,1)
            |  /                |  /
            | /                 | /
            |/                  |/
    (-1,-1,-1) ----------- (1,-1,-1)


            6 ----------- 4
           /|            /|
          / |           / |
         1 ----------- 2  |
         |  |          |  |
         |  7 ---------|--5
         | /           | /
         |/            |/
         0 ----------- 3

    */

    modelPoints.emplace_back(-1, -1, -1);  // 0
    modelPoints.emplace_back(-1, 1, -1);  // 1
    modelPoints.emplace_back(1, 1, -1);  // 2
    modelPoints.emplace_back(1, -1, -1);  // 3
    modelPoints.emplace_back(1, 1, 1);  // 4
    modelPoints.emplace_back(1, -1, 1);  // 5
    modelPoints.emplace_back(-1, 1, 1);  // 6
    modelPoints.emplace_back(-1, -1, 1);  // 7

    // front
    meshFaces.emplace_back(0, 1, 2);
    meshFaces.emplace_back(0, 2, 3);

    // right
    meshFaces.emplace_back(3, 2, 4);
    meshFaces.emplace_back(3, 4, 5);

    // back
    meshFaces.emplace_back(5, 4, 6);
    meshFaces.emplace_back(5, 6, 7);

    // left
    meshFaces.emplace_back(7, 6, 1);
    meshFaces.emplace_back(7, 1, 0);

    // top
    meshFaces.emplace_back(1, 6, 4);
    meshFaces.emplace_back(1, 4, 2);

    // bottom
    meshFaces.emplace_back(5, 7, 0);
    meshFaces.emplace_back(5, 0, 3);



}

void loadCubeCloud(std::vector<Vector3>& modelPoints, Vector3 position, float inc)
{
    for (float z = position.z; z <= 1.0f; z += inc)
    {
        for (float y = position.y; y <= 1.0f; y += inc)
        {
            for (float x = position.x; x <= 1.0f; x += inc)
            {
                Vector3 vec(x, y, z);

                modelPoints.push_back(vec);
            }
        }
    }
}

void loadTriangle(std::vector<Vector3>& modelPoints, std::vector<Face>& meshFaces)
{
    modelPoints.emplace_back(-1, -1, -1);  // 0
    modelPoints.emplace_back(-1,  1, -1);  // 1
    modelPoints.emplace_back( 1,  1, -1);  // 2
       
    meshFaces.emplace_back(0, 1, 2);    
}
