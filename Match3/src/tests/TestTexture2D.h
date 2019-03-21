#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test
{
	class TestTexture2D: public Test
	{
		public:
			TestTexture2D();
			~TestTexture2D();

			void OnUpdate(float deltaTime) override;
			void OnRender() override;
			void OnImGuiRender() override;

		private:
			//float m_ClearColor[4];
			std::unique_ptr<VertexArray> m_VAO;
			std::unique_ptr<VertexBuffer> m_VertexBuffer;
			std::unique_ptr<VertexBuffer> m_ModelMatrixBuffer;
			std::unique_ptr<IndexBuffer> m_IndexBuffer;
			std::unique_ptr<Shader> m_Shader;
			std::unique_ptr<Texture> m_Texture;

			glm::vec3 m_TranslationA, m_TranslationB;
			glm::mat4 m_Proj, m_View;
			float m_Positions[8];
			std::unique_ptr<VertexBuffer> m_TranslationBuffer;
			//VertexBufferLayout layout;
	};

}