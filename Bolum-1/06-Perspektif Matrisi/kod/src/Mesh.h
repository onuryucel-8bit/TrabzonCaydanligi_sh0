#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
    
    std::vector<Vertex> vertices;

    glm::mat4 S;
                
    glm::mat4 Rx;
    glm::mat4 Ry;
    glm::mat4 Rz;
                
    glm::mat4 T;

    glm::vec3 alfa;
    glm::vec3 pozisyon;
    glm::vec3 olcek;
private:

};

