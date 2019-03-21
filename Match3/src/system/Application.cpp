//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "MainWindow.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application() : m_MainWindow(0)
{
	s_Instance = this;
}

Application::~Application()
{
}

void Application::Initialise()
{
	m_MainWindow = new MainWindow();


	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	//Window* mainWindow = new Window();
	/* Create a windowed mode window and its OpenGL context */
	//window = mainWindowglfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!m_MainWindow->GetGLFWindow())
	{
		glfwTerminate();
		//return -1;
	}

	/* Make the window's context current */
	//glfwMakeContextCurrent(window);

	//glewInit();

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

		ImGui::CreateContext();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(m_MainWindow->GetGLFWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		currentTest = nullptr;
		testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("Texture2D");

		m_Game = new Game();
		m_Graphics = new Graphics();
		m_MouseInput = new MouseInput(m_MainWindow);
		m_Game->Initialise(m_Graphics);
}

void Application::Run()
{
	m_LastUpdateTick = glfwGetTime();
	/* Loop until the user closes the window */
	while (!m_MainWindow->ShouldClose())
	{
		float now = static_cast<float>(glfwGetTime());
		float deltaTime = now - m_LastUpdateTick;
		m_LastUpdateTick = now;
		
		Update(deltaTime);
		Render();
		

	}
}

void Application::Terminate()
{
	delete currentTest;
	if (currentTest != testMenu)
	{
		delete testMenu;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete m_Graphics;
	delete m_Game;
	delete m_MainWindow;
	delete m_MouseInput;
}

void Application::Update(float deltaTime)
{
	m_Game->Update(deltaTime);
	m_MainWindow->Update();
}

void Application::Render()
{
	/* Render here */
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	//renderer.Clear();

	m_Game->Render(m_Graphics);


	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (currentTest)
	{
		currentTest->OnUpdate(0.0f);
		currentTest->OnRender();
		ImGui::Begin("Test");
		if (currentTest != testMenu && ImGui::Button("<-"))
		{
			delete currentTest;
			currentTest = testMenu;
		}
		currentTest->OnImGuiRender();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

