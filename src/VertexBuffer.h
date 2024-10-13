#pragma once

class VertexBuffer {

private:
	unsigned int m_RendererID; //internal id openGL uses to identify objects (shaders, vaos, buffers, etcs)
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	const void Bind();
	const void Unbind();
};