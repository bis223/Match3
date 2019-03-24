#pragma once

#include "StateMachine.h"
#include "MainWindow.h"
#include "Game.h"
#include "Graphics.h"
#include "MouseInput.h"
#include "AssetLoader.h"


class System
{
private:
	static System* s_Instance;

public:

	System();
	~System();

	void Initialise();
	void Run();
	void Terminate();

	inline static System& GetSystemInstance() { return *s_Instance; }

	inline StateMachine* GetStateMachine() const { return m_StateMachine; }
	inline MainWindow* GetMainWindow() const { return m_MainWindow; }
	inline Graphics* GetGraphics() const { return m_Graphics; }
	inline AssetLoader* GetAssetLoader() const { return m_AssetLoader; }
	inline MouseInput* GetMouseInput() const { return m_MouseInput; }
	inline Game* GetGame() const { return m_Game; }



private:
	void Update(float deltaTime);
	void Render();



	double m_LastUpdateTick;

	StateMachine* m_StateMachine;
	MainWindow* m_MainWindow;
	Graphics* m_Graphics;
	AssetLoader* m_AssetLoader;
	MouseInput* m_MouseInput;
	Game* m_Game;
};
