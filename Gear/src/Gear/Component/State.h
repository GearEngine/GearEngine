#pragma once

#include "Component.h"

#define ToString(type) #type
namespace Gear {

	class State : public Component
	{
	public:
		void RegisterStates(const std::initializer_list<std::pair<int, std::string>>& states);
		inline std::string GetCurrentState() { return m_States[m_CurrentState]; }
		inline void SetCurrentState(int state) { m_CurrentState = state; }

	private:
		std::unordered_map<int, std::string> m_States;
		int m_CurrentState = 0;
	};

}