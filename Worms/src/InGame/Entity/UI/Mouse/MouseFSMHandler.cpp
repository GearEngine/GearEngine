#include "wmpch.h"
#include "MouseFSMHandler.h"

namespace InGame {

	std::pair<float, float> m_CurrentPosition;
	std::pair<float, float> m_QuitPosition;
	std::pair<float, float> m_VirtualItemMousePos;

	void MouseOnItemWindowHandler::init(int entityID)
	{
		timer = Gear::EntitySystem::GetTimer(entityID);

		worldID = Gear::EntitySystem::GetEntityIDFromName("World");
		worldStatus = Gear::EntitySystem::GetStatus(worldID);

		m_VirtualItemMousePos.first = 0.65f;
		m_VirtualItemMousePos.second = -0.7f;
		inFirst = false;
	}

	void MouseOnItemWindowHandler::reset()
	{
		curTeamColor = std::any_cast<TeamColor::Color>(worldStatus->GetStat(WorldInfo::CurrentTeamColor));
		switch (curTeamColor)
		{
		case TeamColor::Red:
			virtualItemSelectCursor = Gear::TextureStorage::GetAnimation2D("RedCursor");
			break;
		case TeamColor::Blue:
			virtualItemSelectCursor = Gear::TextureStorage::GetAnimation2D("BlueCursor");
			break;
		}

		virtualItemSelectCursor->SetFrameY(12);

		mouseTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_VirtualItemMousePos.first, m_VirtualItemMousePos.second, ZOrder::z_FlatMouse))
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.16f, 0.16f, 1.0f));

		needReset = false;
	}

	Gear::EnumType MouseOnItemWindowHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		if (inFirst)
		{
			init(entityID);
		}

		if (needReset)
		{
			reset();
		}

		auto[x, y] = Gear::Input::GetMousePosition();

		float dx = x - 640;
		float dy = 360 - y;

		if (std::abs(dx) + std::abs(dy) != 0.0f)
		{
			m_VirtualItemMousePos.first += dx * MouseSensitiveX;
			m_VirtualItemMousePos.second += dy * MouseSensitiveY;

			mouseTranslate[3][0] = m_VirtualItemMousePos.first;
			mouseTranslate[3][1] = m_VirtualItemMousePos.second;
		}

		Gear::Renderer2D::DrawFixedAnimation(mouseTranslate, virtualItemSelectCursor);

		glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);

		if (timer->isExpired())
		{
			if (cmd.Keycode == GR_MOUSE_BUTTON_LEFT)
			{
				timer->SetTimer(LeftClickDelay);
				timer->Start();
				Gear::EventSystem::DispatchEvent(EventChannel::MouseClick, Gear::EntityEvent(EventType::MouseClick, GR_MOUSE_BUTTON_LEFT));
			}
			if (cmd.Keycode == GR_MOUSE_BUTTON_RIGHT)
			{
				//TODO : Add item select window
				timer->SetTimer(RightClickDelay);
				timer->Start();
				Gear::EventSystem::DispatchEvent(EventChannel::MouseClick, Gear::EntityEvent(EventType::MouseClick, GR_MOUSE_BUTTON_RIGHT));
			}
		}
		return WorldState::OnItemWindow;
	}
}

