#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size, bool staticUsage);
	~VertexBuffer();

	void Bind()const;
	void UnBind()const;
	void SetData(const void * data, unsigned int size, bool staticUsage);
};
