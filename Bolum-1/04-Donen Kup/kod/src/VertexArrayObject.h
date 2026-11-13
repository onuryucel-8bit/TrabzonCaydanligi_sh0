#pragma once

#include <glad/glad.h>

class VertexArrayObject
{
public:
	void genBuffer();
	void loadAttrib(GLuint index, GLint size, GLsizei stride);
	void unbind();
	void bind();

	GLuint m_id;
};