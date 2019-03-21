#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
				  x;\
				  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Clear()const;
	void DrawInstanced(const VertexArray& va, IndexBuffer& ib, const Shader& shader, int instanceCount) const;
	void Draw(const VertexArray& va, IndexBuffer& ib, const Shader& shader) const;
};

