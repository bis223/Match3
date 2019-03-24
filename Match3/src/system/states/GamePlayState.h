#pragma once

#include "IState.h"

class GamePlayState : public IState
{
public:
	GamePlayState();
	~GamePlayState();

	virtual void Activate() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Graphics* graphics) override;
	virtual void Deactivate() override;

private:

};
