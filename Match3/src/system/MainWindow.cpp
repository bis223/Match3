#include "MainWindow.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

MainWindow::MainWindow():m_GlfWindow(0)
{
	/* Initialize the library */
	glfwInit();
	
	m_GlfWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL); 

	/* Make the window's context current */
	glfwMakeContextCurrent(m_GlfWindow);

	//glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	glfwSetInputMode(m_GlfWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);

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

	ImGui::CreateContext();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_GlfWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

MainWindow::~MainWindow()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow();
}

bool MainWindow::ShouldClose()
{
	return glfwWindowShouldClose(m_GlfWindow);
}

void MainWindow::Update()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_GlfWindow);

	/* Poll for and process events */
	glfwPollEvents();
}

void MainWindow::Render()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("ImGui");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//void MainWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//}

int MainWindow::GetMouseButtonState(int action)
{
	/*int state = 0;
	
	switch (action)
	{
	case GLFW_MOUSE_BUTTON_LEFT:

	default:
		break;
	}
	glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
*/
	return glfwGetMouseButton(m_GlfWindow, action);
}

void MainWindow::GetCursorPosition(double& xPos, double& yPos)
{
	glfwGetCursorPos(m_GlfWindow, &xPos, &yPos);
}

void MainWindow::CreateWindow(GLFWwindow* glfWindow)
{
	/* Create a windowed mode window and its OpenGL context */
	//GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
	glfWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
	/*if (!m_Window)
	{
		glfwTerminate();
		return -1;
	}*/

	///* Make the window's context current */
	//glfwMakeContextCurrent(m_Window);

	//return window;
}

void MainWindow::DestroyWindow()
{
	glfwTerminate();
}
