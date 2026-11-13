#pragma once

#include <glad/glad.h>

class VertexBufferObject
{
public:
	void genBuffer();
	void loadData(GLsizeiptr size, float* data);
	void loadData(GLsizeiptr size, glm::vec3* data);
	void unbind();

	GLuint m_id;
};