

#include "Graphics.h"
#include "VertexBufferLayout.h"

#include <iostream>

Graphics::Graphics():m_Projection(0), m_View(0)
{
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	m_InstancedShader	= std::make_unique<Shader>("res/shaders/Instanced.shader");
	m_BasicShader		= std::make_unique<Shader>("res/shaders/Basic.shader");
	m_TextShader		= std::make_unique<Shader>("res/shaders/Text.shader");
}

Graphics::~Graphics()
{
}

void Graphics::SetProjectionMatrix(const glm::mat4& projection)
{
	m_Projection = projection;
}

void Graphics::SetViewMatrix(const glm::mat4 & view)
{
	m_View = view;
}

void Graphics::ClearColor()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::DrawInstancedTexture(const void* staticData, unsigned int staticDataSize, const void* dynamicData, unsigned int dynamicDataSize, const Texture* texture, int instanceCount)
{

	VertexBuffer dynamicBuffer = VertexBuffer(dynamicData, dynamicDataSize, false);


	unsigned int indices[] = 
	{
								0,1,2,
								2,3,0
	};

	EnableBlending();

	VertexArray VAO;// = std::make_unique<VertexArray>();
	VertexBuffer staticBuffer = VertexBuffer(staticData, staticDataSize, true);

	VertexBufferLayout layout;
	layout.Push<float>(2);	// vertex positions
	layout.Push<float>(2);  // texture coordinates
	VAO.Bind();
	VAO.AddBuffer(staticBuffer, layout, 0);
	staticBuffer.UnBind();
	//for (unsigned int i = 0; i < amount; i++)
	{


		VertexBufferLayout translationLayout;
		translationLayout.Push<float>(4);
		translationLayout.Push<float>(4);
		translationLayout.Push<float>(4);
		translationLayout.Push<float>(4);
		//translationLayout.Push<glm::mat4>(1);
		VAO.AddBuffer(dynamicBuffer, translationLayout, 2);

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		//glBindVertexArray(0);
		VAO.UnBind();
	}


	IndexBuffer indexBuffer = IndexBuffer(indices, 6);


	Renderer renderer;
	texture->Bind(); // slot 0 by default

	m_InstancedShader->Bind();
	m_InstancedShader->SetUniform1i("u_Texture", 0); // pass slot

	m_InstancedShader->SetUniformMat4f("u_Projection", m_Projection);
	m_InstancedShader->SetUniformMat4f("u_View", m_View);
	renderer.DrawInstanced(VAO, indexBuffer, *m_InstancedShader, instanceCount);

	texture->UnBind();
	m_InstancedShader->UnBind();
	dynamicBuffer.UnBind();
}

void Graphics::DrawStaticTexture(const void * staticData, unsigned int staticDataSize, const Texture * texture)
{

	unsigned int indices[] =
	{
								0,1,2,
								2,3,0
	};

	EnableBlending();

	VertexArray VAO;// = std::make_unique<VertexArray>();
	VertexBuffer staticBuffer = VertexBuffer(staticData, staticDataSize, true);

	VertexBufferLayout layout;
	layout.Push<float>(2);	// vertex positions
	layout.Push<float>(2);  // texture coordinates
	VAO.Bind();
	VAO.AddBuffer(staticBuffer, layout, 0);
	staticBuffer.UnBind();
	VAO.UnBind();
	
	IndexBuffer indexBuffer = IndexBuffer(indices, 6);

	Renderer renderer;
	texture->Bind(); // slot 0 by default

	m_BasicShader->Bind();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(377.5f, 300.0f, 0.0f));
	model = glm::scale(model, glm::vec3(75.5f, 60.0f, 0.0f));
	glm::mat4 mvp = m_Projection * m_View * model;
	m_BasicShader->SetUniformMat4f("u_MVP", mvp);
	m_BasicShader->SetUniform1i("u_Texture", 0); // pass slot

	renderer.Draw(VAO, indexBuffer, *m_BasicShader);
}

void Graphics::DrawString(Shader & shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
}

void Graphics::EnableBlending()
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
