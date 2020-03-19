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
		MouseUpdate(ts);

		for (auto& command : m_Commands)
		{
			if (command.Keycode == MOUSEMOVE_COMMAND)
			{
				m_Command.KeyType = command.KeyType;
				break;
			}

			if (Gear::Input::IsKeyPressd(command.Keycode))
			{
				m_Command = command;
				break;
			}
		}
	}
	void Controller::MouseUpdate(Timestep ts)
	{
		auto currentMousePos = Gear::Input::GetMousePosition();
		currentMousePos.second = 720 - currentMousePos.second;

		if (m_PrevMousePos.first == 0.0f && m_PrevMousePos.second == 0.0f)
		{
			m_PrevMousePos = currentMousePos;
			return;
		}

		if (currentMousePos != m_PrevMousePos)
		{
			m_Command.Keycode = MOUSEMOVE_COMMAND;
			m_Command.MouseMove = { 
				currentMousePos.first - m_PrevMousePos.first, 
				currentMousePos.second - m_PrevMousePos.second 
			};
			m_PrevMousePos = currentMousePos;
		}

	}
}