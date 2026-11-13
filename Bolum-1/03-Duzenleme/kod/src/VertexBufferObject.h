#pragma once

#include <glad/glad.h>

class VertexBufferObject
{
public:
	void genBuffer();
	void loadData(GLsizeiptr size, float* data);
	void unbind();

	GLuint m_id;
};