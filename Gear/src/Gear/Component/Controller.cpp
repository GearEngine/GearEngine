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


	bool NetController::SendOnlyNull = false;
	Ref<NetController::EventController> NetController::s_EventController;
	std::vector<int> NetController::s_AcceptableEntity;
	bool NetController::s_inputAcceptable = false;
	NetController::~NetController()
	{
		m_Commands.clear();
		m_EventCotroller.reset();
	}

	void NetController::Update(Timestep ts)
	{
		NetWorkManager::Get()->Send<Command>(Controller::s_None);
	}

	void NetController::SendInput()
	{
		if (!m_OnActivate)
		{
			return;
		}

		bool send = false;
		bool isAcceptable = false;
		for (int i = 0; i < accepterbleEntity.size(); ++i)
		{
			if (m_ID == accepterbleEntity[i])
			{
				isAcceptable = true;
				break;
			}
		}
		if (!isAcceptable)
		{
			return;
		}

		for (auto& command : m_Commands)
		{
			if (command.KeyType == MOUSE_CLICKTYPE && m_ActivatedMouse)
			{
				if (Gear::Input::IsMouseButtonPressed(command.Keycode))
				{
					NetWorkManager::Get()->Send<Command>(command);
					send = true;
					break;
				}
			}
			else
			{
				if (Gear::Input::IsKeyPressd(command.Keycode))
				{
					NetWorkManager::Get()->Send<Command>(command);
					send = true;
					break;
				}
			}
		}

		if (!send)
		{
			SendNull();
		}
	}

	void NetController::ReceiveInput()
	{
		auto data = NetWorkManager::Get()->ReceiveInputStream();
		if (data.second == 0)
		{
			return;
		}
		char* buffer = static_cast<char*>(malloc(data.second));
		memcpy(buffer, data.first, data.second);
		InputMemoryStream checkStream(data.first, data.second);

		m_TypeChecker.Read(checkStream);
		auto type = m_TypeChecker.m_Type;

		InputMemoryStream inputStream(buffer, data.second);
		switch (type)
		{
		case 0:
			m_Command.Read(inputStream);
			break;
		case 1:
			break;
		case 2:
			m_EventCotroller->Handle(m_ID, inputStream);
			break;
		}
	}

	void NetController::SendNull()
	{
		if (s_inputAcceptable)
		{
			NetWorkManager::Get()->Send<Command>(Controller::s_None);
		}
	}

	void NetController::staticReceive()
	{
		TypeChecker tc;
		auto data = NetWorkManager::Get()->ReceiveInputStream();
		std::cout << "Receive packet! On static Receive" << std::endl;
		if (data.second == 0)
		{
			return;
		}
		char* buffer = static_cast<char*>(malloc(data.second));
		memcpy(buffer, data.first, data.second);
		InputMemoryStream checkStream(data.first, data.second);

		tc.Read(checkStream);
		auto type = tc.m_Type;

		std::cout << "type : " << type << std::endl;

		InputMemoryStream inputStream(buffer, data.second);
		if(type == 2)
		{
			//std::cout << "Get event packet! turn change" << std::endl;
			s_EventController->Handle(-1, inputStream);
		}
	}

}