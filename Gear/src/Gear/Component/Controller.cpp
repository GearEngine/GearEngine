#include "grpch.h"
#include "Controller.h"
#include "Gear/Core/Input.h"

namespace Gear {

	Command Controller::s_None = { NONE_COMMAND, NONE_COMMAND };

	Controller::~Controller()
	{
	}

	void Controller::Update(Timestep ts)
	{
		for (auto& command : m_Commands)
		{
			if (command.KeyType == MOUSE_CLICKTYPE)
			{
				if (Gear::Input::IsMouseButtonPressed(command.Keycode))
				{
					m_Command = command;
					break;
				}
			}
			else 
			{
				if (Gear::Input::IsKeyPressd(command.Keycode))
				{
					m_Command = command;
					break;
				}
			}
		}
	}

}