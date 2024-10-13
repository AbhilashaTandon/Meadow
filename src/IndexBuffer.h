#pragma once

class IndexBuffer {

private:
	unsigned int m_RendererID; //internal id openGL uses to identify objects (shaders, vaos, buffers, etcs)
	unsigned int m_Count; //number of indices
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	const void Bind();
	const void Unbind();

	inline unsigned int GetCount() const { return m_Count;  }
};