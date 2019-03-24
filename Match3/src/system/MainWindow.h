#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 755.0f
#define SCREEN_HEIGHT 600.0f

class MainWindow
{
public:
	MainWindow();
	~MainWindow();
	bool ShouldClose();
	void Update();
	void Render();
	inline GLFWwindow* GetGLFWindow() const { return m_GlfWindow; }
	//void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	int GetMouseButtonState(int action);
	void GetCursorPosition(double& xPos, double& yPos);
private:
	void CreateWindow(GLFWwindow* glfWindow);
	void DestroyWindow();

	GLFWwindow* m_GlfWindow;
};