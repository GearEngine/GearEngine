#include "grpch.h"
#include "FSM.h"
#include "Controller.h"

namespace Gear {

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

	void FSM::RegisterFSM(const std::initializer_list<std::pair<const EnumType, InputHandler*>>& handlers)
	{
		m_Handlers = handlers;
		for (auto& handle : handlers)
		{
			m_States.push_back(handle.first);
		}
	}

	void FSM::Handle(const Command& cmd)
	{
		m_CurrentState = m_Handlers[m_CurrentState]->Handle(cmd);
	}
}
