#pragma once

#include <string>
class Graphics;

class IState
{
public:
	//IState();
	virtual ~IState() {}

	virtual void Activate() = 0;
	virtual void Update(float deltaTime) {};
	virtual void Render(Graphics* graphics) {};
	virtual void Deactivate() = 0;
	//inline const std::string& GetStateId() const { return m_StateId; }
protected:
	bool m_IsActive;
	//std::string m_StateId;

};

