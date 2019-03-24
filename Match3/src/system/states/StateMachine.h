#pragma once

#include <string>
#include <map>

class IState;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void AddState(const std::string &stateId, IState* state);
	void SetState(const std::string &stateId);
	const IState* GetState(const std::string &stateId);
	inline IState* GetCurrentState()  { return m_CurrentState; }


private:

	typedef std::map<std::string, IState*> StateMap;
	StateMap m_StateMap;
	IState* m_CurrentState;
};
