#include "VertexBuffer.h"
#include "Renderer.h"
VertexBuffer::VertexBuffer(const void * data, unsigned int size, bool staticUsage)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	 
	glBufferData(GL_ARRAY_BUFFER, size, data, staticUsage == true ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SetData(const void * data, unsigned int size, bool staticUsage)
{
	//Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, staticUsage == true ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

void VertexBuffer::Bind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
