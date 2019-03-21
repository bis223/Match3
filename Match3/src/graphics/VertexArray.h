#pragma once

#include "VertexBuffer.h"
//#include "VertexBufferLayout.h"
class VertexBufferLayout;

class VertexArray
{

private:
	unsigned int m_RendererID;

public:
	 VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int pointerBegin);

	void Bind() const;
	void UnBind() const;
};
