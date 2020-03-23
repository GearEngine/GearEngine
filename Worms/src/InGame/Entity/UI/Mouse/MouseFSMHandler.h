#pragma once

#include "InGame/Entity/World/WorldState.h"
#include "InGame/Data/EventData.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace InGame {

	class MouseOnGameRunningHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static bool first = false;
			static float prevDx = 0.0f;
			static float prevDy = 0.0f;
			if (cmd.Keycode == GR_MOUSE_BUTTON_LEFT)
			{
				return WorldState::OnRunning;
			}
			if (cmd.Keycode == GR_MOUSE_BUTTON_RIGHT)
			{
				//TODO : Add item select window
				return WorldState::OnRunning;
			}

			if (first)
			{
				auto[x, y] = Gear::Input::GetMousePosition();

				float dx = x - 640;
				float dy = 360 - y;
				if (std::abs(dx) + std::abs(dy) != 0.0f)
				{
					dx = (dx + prevDx) * 0.5f;
					dy = (dy + prevDy) * 0.5f;
					Gear::EventSystem::DispatchEvent(EventChannel::MouseMove, Gear::EntityEvent(EventType::MouseMove, MouseMoveData(dx, dy)));
				}
				prevDx = dx;
				prevDy = dy;
			}

			glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);
			first = true;

			return WorldState::OnRunning;
		}
	};

	class MouseOnItemWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
		
			return WorldState::OnItemWindow;
		}
	};

	class MouseOnWindowSelectHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			
			return WorldState::OnWindowSelect;
		}
	};

	class MouseOnQuitWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			
			return WorldState::OnQuitWindow;
		}
	};


}