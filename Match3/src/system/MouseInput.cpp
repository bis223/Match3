#include "MouseInput.h"

MouseInput::MouseInput(MainWindow* mainWindow): m_MainWindow(mainWindow)
{

}

MouseInput::~MouseInput()
{
}

bool MouseInput::GetMouseButtonDown(int action)
{
	int state = m_MainWindow->GetMouseButtonState(action);
	return state == GLFW_PRESS;
}

bool MouseInput::GetMouseButtonUp(int action)
{
	int state = m_MainWindow->GetMouseButtonState(action);
	return state == GLFW_RELEASE;
}

void MouseInput::GetMousePosition(double& xPos, double& yPos)
{
	m_MainWindow->GetCursorPosition(xPos, yPos);
}
