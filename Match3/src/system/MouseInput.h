#pragma once
#include "MainWindow.h"

class MouseInput
{
public:
	MouseInput(MainWindow* mainWindow);
	~MouseInput();

	bool GetMouseButtonDown(int action);
	bool GetMouseButtonUp(int action);
	void GetMousePosition(double& xPos, double& yPos);

private:
	MainWindow* m_MainWindow;
};


