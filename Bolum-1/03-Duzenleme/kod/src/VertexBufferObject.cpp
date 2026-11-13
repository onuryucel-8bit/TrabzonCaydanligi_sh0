#include "VertexBufferObject.h"

void VertexBufferObject::genBuffer()
{
	glad_glGenBuffers(1, &m_id);
}

void VertexBufferObject::loadData(GLsizeiptr size, float* data)
{
	//vbo ya ne oldugu soyle 
	glad_glBindBuffer(GL_ARRAY_BUFFER, m_id);
	//vbo ya veriyi yolla
	glad_glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBufferObject::unbind()
{
	//vbo baglantisini kes
	glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
}
