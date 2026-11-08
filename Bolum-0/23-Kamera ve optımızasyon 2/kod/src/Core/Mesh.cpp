#include "Mesh.h"

Mesh::Mesh()
{
    alfa = glm::vec3(0);
    olcek = glm::vec3(1);
    pozisyon = glm::vec3(0);

    S = glm::scale(glm::mat4(1.0f), olcek);

    Rx = glm::rotate(glm::mat4(1.0f), glm::radians(alfa.x), glm::vec3(1.0f, 0.0f, 0.0f));
    Ry = glm::rotate(glm::mat4(1.0f), glm::radians(alfa.y), glm::vec3(0.0f, 1.0f, 0.0f));
    Rz = glm::rotate(glm::mat4(1.0f), glm::radians(alfa.z), glm::vec3(0.0f, 0.0f, 1.0f));

    T = glm::translate(glm::mat4(1.0f), pozisyon);
   /* = glm::scale(glm::mat4(1.0f), olcek);

    = glm::rotate(glm::mat4(1.0f), glm::radians(alfa.x), glm::vec3(1.0f, 0.0f, 0.0f));
    = glm::rotate(glm::mat4(1.0f), glm::radians(alfa.y), glm::vec3(0.0f, 1.0f, 0.0f));
    = glm::rotate(glm::mat4(1.0f), glm::radians(alfa.z), glm::vec3(0.0f, 0.0f, 1.0f));

    = glm::translate(glm::mat4(1.0f), pozisyon);*/
}

Mesh::~Mesh()
{
}