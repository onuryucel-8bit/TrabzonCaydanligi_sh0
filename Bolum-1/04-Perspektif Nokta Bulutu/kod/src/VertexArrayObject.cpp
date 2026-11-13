#include "VertexArrayObject.h"

void VertexArrayObject::genBuffer()
{
	glad_glGenVertexArrays(1, &m_id);
}

void VertexArrayObject::loadAttrib(GLuint index, GLint size, GLsizei stride)
{
    //nokta verisinin bellekte nasil tutuldugunu bildir
    //3 * sizeof(float)
    glad_glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
    //vertex attrib sifiri aktif hale getir

    //vertexShader icindeki location = 0 konumuna x,y,z pozisyon bilgileri buray gidicek diyoruz 
    //layout (location = 0) in vec3 aPos;
    glad_glEnableVertexAttribArray(0);
}

void VertexArrayObject::unbind()
{
    //vao baglantisini kes
    glad_glBindVertexArray(0);
}

void VertexArrayObject::bind()
{
	//vao => vbo bagla
	glad_glBindVertexArray(m_id);
}
