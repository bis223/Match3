#pragma once
#include "MainWindow.h"
#include "tests/Test.h"
#include "Game.h"
#include "Graphics.h"
#include "MouseInput.h"

class Application
{
private:
	static Application* s_Instance;

public:

	Application();
	~Application();

	void Initialise();
	void Run();
	void Terminate();

	inline static Application& GetApplication() { return *s_Instance; }
	inline Game* GetGame() const { return m_Game; }
	inline Graphics* GetGraphics() const { return m_Graphics; }
	inline MouseInput* GetMouseInput() const { return m_MouseInput; }



private:
	void Update(float deltaTime);
	void Render();


	MainWindow* m_MainWindow;

	//Renderer renderer;
	test::Test* currentTest;
	test::TestMenu* testMenu;

	double m_LastUpdateTick;

	Game* m_Game;
	Graphics* m_Graphics;
	MouseInput* m_MouseInput;
};
