#include "MainWindow.h"



MainWindow::MainWindow():m_Window(0)
{
	/* Initialize the library */
	glfwInit();
	
	m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL); 

	/* Make the window's context current */
	glfwMakeContextCurrent(m_Window);

	//glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
	glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, 1);
}

MainWindow::~MainWindow()
{
	DestroyWindow();
}

bool MainWindow::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void MainWindow::Update() const
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_Window);

	/* Poll for and process events */
	glfwPollEvents();
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
	return glfwGetMouseButton(m_Window, action);
}

void MainWindow::GetCursorPosition(double& xPos, double& yPos)
{
	glfwGetCursorPos(m_Window, &xPos, &yPos);
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
