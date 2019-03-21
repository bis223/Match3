#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
		//:m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f }
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		  m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{

		float vertices[] = {
			-5.0f, -5.0f, 0.0f, 0.0f,	//0
			 5.0f, -5.0f, 1.0f, 0.0f,	//1
			 5.0f, 5.0f,  1.0f, 1.0f,	//2
			-5.0f, 5.0f,  0.0f, 1.0f,	//3
		};

		//float vertices[] = {
		//			-50.0f, -50.0f, 0.0f, 0.0f,	//0
		//			 50.0f, -50.0f, 0.5f, 0.0f,	//1
		//			 50.0f, 50.0f, 0.5f, 0.5f,	//2
		//			-50.0f, 50.0f, 0.0f, 0.5f,	//3
		//			 150.0f, 150.0f, 0.5f, 0.5f,//0
		//			 250.0f, 150.0f, 1.0f, 0.5f,//1
		//			 250.0f, 250.0f, 1.0f, 1.0f,//2
		//			 150.0f, 250.0f, 0.5f, 1.0f,//3
		//};

		//		float vertices[] = {
		//			-0.05f, -0.05f,  0.0f, 0.0f,	//0
		//			 0.5f, -0.05f,   0.5f, 0.0f,	//1
		//			 0.05f, 0.05f,   0.5f, 0.5f,	//2
		//			-0.05f, 0.05f,   0.0f, 0.5f,	//3
		//			 150.0f, 150.0f, 0.5f, 0.5f,//0
		//			 250.0f, 150.0f, 1.0f, 0.5f,//1
		//			 250.0f, 250.0f, 1.0f, 1.0f,//2
		//			 150.0f, 250.0f, 0.5f, 1.0f,//3
		//};

		/*-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f*/
		/*for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				m_Positions[i] = i + 0;
				m_Positions[j] = j + 0;
			}
		}*/

		unsigned int amount = 4;
		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[amount];
		//srand(glfwGetTime()); // initialize random seed	
		float radius = 150.0;
		float offset = 25.0f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			float x = i * 100;
			float y = i * 100; 
			float z = 0;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			//float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(15));

			 //3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			//float rotAngle = (rand() % 360);
			model = glm::rotate(model, 0.0f, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}

		/*unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);*/

		m_ModelMatrixBuffer = std::make_unique<VertexBuffer>(&modelMatrices[0], amount * sizeof(glm::mat4), false);

	
		unsigned int indices[] = {
									0,1,2,
									2,3,0,/*
									4,5,6,
									6,7,4*/
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float), true);
		VertexBufferLayout layout;
		layout.Push<float>(2);	// vertex positions
		layout.Push<float>(2);  // texture coordinates
		m_VAO->Bind();
		m_VAO->AddBuffer(*m_VertexBuffer, layout, 0);
		m_VertexBuffer->UnBind();
		//for (unsigned int i = 0; i < amount; i++)
		{
			//unsigned int VAO;// = rock.meshes[i].VAO;
			//glGenVertexArrays(1, &VAO);
			//glBindVertexArray(VAO);
			// set attribute pointers for matrix (4 times vec4)


			//glBindBuffer(GL_ARRAY_BUFFER, buffer);
			//glEnableVertexAttribArray(3);
			//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			//glEnableVertexAttribArray(4);
			//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			//glEnableVertexAttribArray(5);
			//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			//glEnableVertexAttribArray(6);
			//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));


			VertexBufferLayout translationLayout;
			translationLayout.Push<glm::mat4>(1);
			m_VAO->AddBuffer(*m_ModelMatrixBuffer, translationLayout, 2);

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			//glBindVertexArray(0);
			m_VAO->UnBind();
		}

		//m_TranslationBuffer = std::make_unique<VertexBuffer>(m_Positions, 2 * 4 * sizeof(float));
		//VertexBufferLayout translationLayout;
		//translationLayout.Push<float>(2);	// vertex positions
		////layout.Push<float>(2);  // texture coordinates
		//m_VAO->AddBuffer(*m_TranslationBuffer, translationLayout);
		//glVertexAttribDivisor(2, 1);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Texture = std::make_unique<Texture>("res/textures/bis.jpg");
		
		m_Shader = std::make_unique<Shader>("res/shaders/Instanced.shader");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0); // pass slot
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		//GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_Shader->Bind();

		Renderer renderer;
		m_Texture->Bind(); // slot 0 by default


		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[4];
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			float x = 0;
			if(i == 0)
			{
				x = m_TranslationA.x;
			}
			else
			{
				x = i * 100;
			}
			float y = i * 100;
			float z = 0;

			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			//float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(15));

			//3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		   //float rotAngle = (rand() % 360);
			model = glm::rotate(model, 0.0f, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			modelMatrices[i] = model;
		}
		//std::unique_ptr<VertexBuffer> modelMatrixBuffer = std::make_unique<VertexBuffer>(&modelMatrices[0], 4 * sizeof(glm::mat4), false);
		m_ModelMatrixBuffer->SetData(&modelMatrices[0], 4 * sizeof(glm::mat4), false);

		//m_Positions[0] = glm::vec3(m_TranslationA);
		{
			//glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			//glm::mat4 mvp = m_Proj * m_View * model;
			//m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniformMat4f("u_Projection", m_Proj);
			m_Shader->SetUniformMat4f("u_View", m_View);
			//m_Shader->SetUniformMat4f("u_Model", model);
			renderer.DrawInstanced(*m_VAO, *m_IndexBuffer, *m_Shader, 4);
		}
		/*{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}*/

	}

	void TestTexture2D::OnImGuiRender()
	{
		//ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}