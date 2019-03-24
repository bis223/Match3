#pragma once

#include "IState.h"

class LoadingState : public IState
{
public:
	LoadingState();
	~LoadingState();

	virtual void Activate() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Graphics* graphics) override;
	virtual void Deactivate() override;

private:

};
