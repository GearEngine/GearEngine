#pragma once

#include "InGame/GameState.h"
#include "InGame/Data/EventData.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace InGame {

	class MouseOnGameRunningHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.Keycode == GR_MOUSE_BUTTON_LEFT)
			{
				return GameState::OnRunning;
			}
			if (cmd.Keycode == GR_MOUSE_BUTTON_RIGHT)
			{
				//TODO : Add item select window
				return GameState::OnRunning;
			}

			auto[x, y] = Gear::Input::GetMousePosition();

			float dx = x - 640;
			float dy = 360 - y;
			Gear::EventSystem::DispatchEvent(EventChannel::MouseMove, Gear::EntityEvent(EventType::MouseMove, MouseMoveData(dx, dy)));

			glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);

			return GameState::OnRunning;
		}
	};

	class MouseOnItemWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
		
			return GameState::OnItemWindow;
		}
	};

	class MouseOnWindowSelectHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			
			return GameState::OnWindowSelect;
		}
	};

	class MouseOnQuitWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			
			return GameState::OnQuitWindow;
		}
	};


}