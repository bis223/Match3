#pragma once
#include <vector>
//#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT_VEC4:		return 16;
		case GL_FLOAT_MAT4:		return 64;
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}

};

class VertexBufferLayout
{
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		VertexBufferLayout(): m_Stride(0){}
	//	~VertexBufferLayout();

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({GL_FLOAT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);// sizeof(GLfloat);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); //sizeof(GLuint);
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); //sizeof(GLbyte);
		}

		template<>
		void Push<glm::mat4>(unsigned int count)
		{
			unsigned int size = sizeof(glm::vec4);
			//m_Elements.push_back({ GL_FLOAT_MAT4, count, GL_FALSE });
			//m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT_MAT4);
			//for (unsigned int i = 0; i < count; i++)
			{
				m_Elements.push_back({ GL_FLOAT, 4, GL_FALSE });
				m_Stride += count * size;

				m_Elements.push_back({ GL_FLOAT, 4, GL_FALSE });
				m_Stride += count * size;

				m_Elements.push_back({ GL_FLOAT, 4, GL_FALSE });
				m_Stride += count * size;

				m_Elements.push_back({ GL_FLOAT, 4, GL_FALSE });
				m_Stride += count * size;
			}


			/*glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));*/
		}

		template<>
		void Push<glm::vec4>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT_VEC4);// sizeof(glm::vec4);
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
};

