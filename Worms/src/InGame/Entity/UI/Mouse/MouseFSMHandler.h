#pragma once

#include "InGame/Entity/World/WorldState.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace InGame {

	static float RightClickDelay = 0.2f;
	static float LeftClickDelay = 0.1f;

	class MouseOnGameRunningHandler : public Gear::FSM::InputHandler
	{

		Gear::Ref<Gear::Timer> timer;
		bool inFirst = true;

		void init(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			timer->Stop();
			inFirst = false;
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static bool first = false;
			static float prevDx = 0.0f;
			static float prevDy = 0.0f;

			if (inFirst)
			{
				init(entityID);
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

			if (timer->isExpired())
			{
				if (cmd.Keycode == GR_MOUSE_BUTTON_RIGHT)
				{
					//TODO : Add item select window
					timer->SetTimer(RightClickDelay);
					timer->Start();
					Gear::EventSystem::DispatchEvent(EventChannel::MouseClick, Gear::EntityEvent(EventType::MouseClick, GR_MOUSE_BUTTON_RIGHT));
					return WorldState::OnItemWindow;
				}
			}

			return WorldState::OnRunning;
		}
	};

	class MouseOnItemWindowHandler : public Gear::FSM::InputHandler
	{
		bool inFirst = true;
		bool needReset = true;
		
		float MouseSensitiveX = 0.001f;
		float MouseSensitiveY = 0.00177f;

		int worldID;

		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Animation2D> virtualItemSelectCursor;
		Gear::Ref<Gear::Status> worldStatus;
		

		glm::mat4 mouseTranslate;

		unsigned int curTeamColor;

		void init(int entityID);

		void reset();

		inline void OnOut(int entityID) override
		{
			needReset = true;
		}

		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
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