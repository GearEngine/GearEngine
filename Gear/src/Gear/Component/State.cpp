#include "grpch.h"
#include "State.h"

namespace Gear {

	void State::RegisterStates(const std::initializer_list<std::pair<int, std::string>>& states)
	{
		for (auto& e : states)
		{
			m_States.insert(e);
		}
	}

}
