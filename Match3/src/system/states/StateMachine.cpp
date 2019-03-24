#include "StateMachine.h"
#include "BootState.h"

StateMachine::StateMachine(): m_CurrentState(0)
{
	AddState("BootState", new BootState());
	SetState("BootState");
}

StateMachine::~StateMachine()
{
	for (StateMap::iterator stateIt = m_StateMap.begin(), end = m_StateMap.end();
		stateIt != end;
		++stateIt)
	{
		delete stateIt->second;
	}

	m_StateMap.clear();
}

void StateMachine::AddState(const std::string & stateId, IState* state)
{
	m_StateMap.insert(std::make_pair(stateId, state));
}

void StateMachine::SetState(const std::string & stateId)
{
	m_CurrentState = nullptr;
	StateMap::const_iterator stateIt = m_StateMap.find(stateId);
	if (stateIt != m_StateMap.end())
	{
		if (m_CurrentState)
		{
			m_CurrentState->Deactivate();
		}
		m_CurrentState = stateIt->second;
		m_CurrentState->Activate();
	}
}

const IState* StateMachine::GetState(const std::string & stateId)
{
	StateMap::const_iterator stateIt = m_StateMap.find(stateId);
	if (stateIt != m_StateMap.end())
	{
		return stateIt->second;
	}

	return nullptr;
}