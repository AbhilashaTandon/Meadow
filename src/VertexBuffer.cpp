#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCheckError(glGenBuffers(1, &m_RendererID));
	GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCheckError(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCheckError(glDeleteBuffers(1, &m_RendererID));
}

const void VertexBuffer::Bind()
{
	GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

const void VertexBuffer::Unbind()
{
	GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
