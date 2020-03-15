#include "grpch.h"
#include "Controller.h"
#include "Gear/Core/Input.h"

namespace Gear {

	Controller::~Controller()
	{
	}

	void Controller::Update(Timestep ts)
	{
		m_Command = m_None;
		for (auto& command : m_Commands)
		{
			if (Gear::Input::IsKeyPressd(command.Keycode))
			{
				m_Command = command;
				return;
			}
		}
	}

}