#include "grpch.h"
#include "Controller.h"
#include "Gear/Core/Input.h"

namespace Gear {

	Command Controller::s_None = { NONE_COMMAND, NONE_COMMAND };

	Controller::~Controller()
	{
		m_Commands.clear();
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

	NetController::~NetController()
	{
		m_Commands.clear();
	}

	void NetController::Update(Timestep ts)
	{
	}

	void NetController::SendInput()
	{
		for (auto& command : m_Commands)
		{
			if (command.KeyType == MOUSE_CLICKTYPE && m_ActivatedMouse)
			{
				if (Gear::Input::IsMouseButtonPressed(command.Keycode))
				{
					NetWorkManager::Get()->Send(command);
					break;
				}
			}
			else
			{
				if (Gear::Input::IsKeyPressd(command.Keycode))
				{
					NetWorkManager::Get()->Send(command);
					break;
				}
			}
		}
	}

	void NetController::ReceiveInput()
	{
		m_Command = NetWorkManager::Get()->Reseive<Command>();
	}

}