#include "grpch.h"
#include "Controller.h"
#include "Gear/Core/Input.h"

namespace Gear {

	void Controller::Update(Timestep ts)
	{
		m_Command = m_None;
		for (auto& e : m_Commands)
		{
			if (Gear::Input::IsKeyPressd(e.Keycode))
			{
				m_Command = e;
				return;
			}
		}
	}

}