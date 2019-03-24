#pragma once

#include "IState.h"

class BootState : public IState
{
public:
	BootState();
	~BootState();

	virtual void Activate() override;
	virtual void Update(float deltaTime) override {}
	virtual void Render(Graphics* graphics) override {}
	virtual void Deactivate() override;

private:

};
