#include "grpch.h"
#include "FSM.h"
#include "Controller.h"

namespace Gear {

	void FSM::State::RegisterStates(const std::initializer_list<std::pair<int, std::string>>& states)
	{
		for (auto& e : states)
		{
			m_States.insert(e);
		}
	}

	FSM::~FSM()
	{
		for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
		{
			delete it->second;
		}
		m_Handlers.clear();
	}

	void FSM::Update(Timestep ts)
	{
	}

	void FSM::Handle(Command cmd)
	{
		m_State.SetCurrentState(m_Handlers[m_CurrentState]->Handle(cmd));
	}

	void FSM::RegisterHandler(const std::initializer_list<std::pair<int, InputHandler*>>& handlers)
	{
		for (auto& e : handlers)
		{
			m_Handlers[e.first] = std::move(e.second);
		}
	}

}
