#pragma once


class ICommand
{

public:
	virtual ~ICommand() {}
	virtual void Prepare()		= 0;
	virtual bool IsReady()		= 0;
	virtual void Execute()		= 0;
	virtual bool IsActive()		= 0;
	virtual void Update(float deltaTime) {};
	virtual bool IsComplete()	= 0;
	virtual void Finish()		= 0;

protected:
	bool m_IsActive;
	bool m_IsComplete;
};
