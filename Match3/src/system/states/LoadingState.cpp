#include "LoadingState.h"
#include "AssetLoader.h"
#include "System.h"
#include "Constants.h"

LoadingState::LoadingState()
{
	//m_StateId = Constants::LOADING_STATE;
}

LoadingState::~LoadingState()
{
}

void LoadingState::Activate()
{
	m_IsActive = true;
}

void LoadingState::Update(float deltaTime)
{
	if (m_IsActive)
	{
		AssetLoader* assetLoader = System::GetSystemInstance().GetAssetLoader();
		assetLoader->Update();
		if (!assetLoader->IsLoading())
		{
			System::GetSystemInstance().GetStateMachine()->SetState(Constants::GAMEPLAY_STATE);
			System::GetSystemInstance().GetGame()->Initialise();
		}
	}
}

void LoadingState::Render(Graphics * graphics)
{
}

void LoadingState::Deactivate()
{
	m_IsActive = false;
}
