
#include "MainWindow.h"

#include "System.h"

#include "IState.h"

System* System::s_Instance = nullptr;

System::System() : m_StateMachine(0), m_MainWindow(0), m_Graphics(0), m_AssetLoader(0), m_MouseInput(0), m_Game(0)
{
	s_Instance = this;

}

System::~System()
{
}

void System::Initialise()
{
	m_StateMachine = new StateMachine();

	m_MainWindow = new MainWindow();


	if (!m_MainWindow->GetGLFWindow())
	{
		//glfwTerminate();
		//return -1;
	}
		
	m_Graphics = new Graphics();
	m_AssetLoader = new AssetLoader();

	m_MouseInput = new MouseInput(m_MainWindow);
	m_Game = new Game();
	m_Game->Initialise(m_Graphics);


}

void System::Run()
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

void System::Terminate()
{

	delete m_MouseInput;
	delete m_Game;
	delete m_Graphics;
	delete m_AssetLoader;
	delete m_MainWindow;
	delete m_StateMachine;
}

void System::Update(float deltaTime)
{
	//m_AssetLoader->Update();
	IState* currentState = m_StateMachine->GetCurrentState();
	currentState->Update(deltaTime);
	/*if (&m_StateMachine->GetCurrentState() == m_StateMachine->GetState(Constants::GAMEPLAY_STATE))
	{
		m_Game->Update(deltaTime);
	}*/
	m_MainWindow->Update();
}

void System::Render()
{
	/* Render here */
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	IState* currentState = m_StateMachine->GetCurrentState();
	currentState->Render(m_Graphics);
	m_MainWindow->Render();


}

