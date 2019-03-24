#include "GamePlayState.h"
#include "Game.h"
#include "System.h"

GamePlayState::GamePlayState()
{
}

GamePlayState::~GamePlayState()
{
}

void GamePlayState::Activate()
{
}

void GamePlayState::Update(float deltaTime)
{
	Game* game = System::GetSystemInstance().GetGame();
	game->Update(deltaTime);
}

void GamePlayState::Render(Graphics* graphics)
{
	Game* game = System::GetSystemInstance().GetGame();
	game->Render(graphics);
}

void GamePlayState::Deactivate()
{
}
