#include "wmpch.h"
#include "MouseFSMHandler.h"

namespace InGame {

	std::pair<float, float> m_CurrentPosition;
	std::pair<float, float> m_QuitPosition;
	std::pair<float, float> m_VirtualItemMousePos;
	std::pair<float, float> m_VirtualItemMousePickPoint;
	std::pair<float, float> m_VirtualWindowSelectPos;
	std::pair<float, float> m_VirtualWindowSelectPickPoint;

	void MouseOnItemWindowHandler::init(int entityID)
	{
		timer = Gear::EntitySystem::GetTimer(entityID);

		worldID = Gear::EntitySystem::GetEntityIDFromName("World");
		worldStatus = Gear::EntitySystem::GetStatus(worldID);

		ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
		ItemSelectorFSM = Gear::EntitySystem::GetFSM(ItemSelectorID);

		GR_TRACE("MouseOnItemWindowHandler::init mousePos set 1.2f, -0.7f");
		m_VirtualItemMousePos.first = 1.2f;
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
		case TeamColor::Green:
			virtualItemSelectCursor = Gear::TextureStorage::GetAnimation2D("GreenCursor");
			break;
		case TeamColor::Yellow:
			virtualItemSelectCursor = Gear::TextureStorage::GetAnimation2D("YellowCursor");
			break;
		case TeamColor::Purple:
			virtualItemSelectCursor = Gear::TextureStorage::GetAnimation2D("PurpleCursor");
			break;
		case TeamColor::Sky:
			virtualItemSelectCursor = Gear::TextureStorage::GetAnimation2D("SkyCursor");
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
		unsigned int itemSelectorState = ItemSelectorFSM->GetCurrentState();
		if (itemSelectorState == 3 || itemSelectorState == 4)
		{
			auto[x, y] = Gear::Input::GetMousePosition();

			float dx = x - 640;
			float dy = 360 - y;

			if (std::abs(dx) + std::abs(dy) != 0.0f)
			{
				m_VirtualItemMousePos.first += dx * MouseSensitiveX;
				m_VirtualItemMousePos.second += dy * MouseSensitiveY;
			}

			itemSelectorRect = std::any_cast<Gear::Util::FRect>(Gear::EntitySystem::GetStatus(ItemSelectorID)->GetStat(1u));
			if (m_VirtualItemMousePos.first < itemSelectorRect.Left)
			{
				m_VirtualItemMousePos.first = itemSelectorRect.Left;
			}
			if (m_VirtualItemMousePos.first > itemSelectorRect.Right)
			{
				m_VirtualItemMousePos.first = itemSelectorRect.Right;
			}
			if (m_VirtualItemMousePos.second < itemSelectorRect.Bottom)
			{
				m_VirtualItemMousePos.second = itemSelectorRect.Bottom;
			}
			if (m_VirtualItemMousePos.second > itemSelectorRect.Top)
			{
				m_VirtualItemMousePos.second = itemSelectorRect.Top;
			}
		}
		mouseTranslate[3][0] = m_VirtualItemMousePos.first;
		mouseTranslate[3][1] = m_VirtualItemMousePos.second;

		m_VirtualItemMousePickPoint.first = mouseTranslate[3][0] - 0.02f;
		m_VirtualItemMousePickPoint.second = mouseTranslate[3][1] + 0.02f;

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

	std::pair<float, float>& GetMousePickPoint()
	{
		return m_VirtualItemMousePickPoint;
	}
	std::pair<float, float>& GetMousePos()
	{
		return m_VirtualItemMousePos;
	}

	std::pair<float, float>& GetWMousePickPoint()
	{
		return m_VirtualWindowSelectPickPoint;
	}

	std::pair<float, float>& GetWMousePos()
	{
		return m_VirtualWindowSelectPos;
	}

	void MouseOnWindowSelectHandler::init(int entityID)
	{
		timer = Gear::EntitySystem::GetTimer(entityID);

		worldID = Gear::EntitySystem::GetEntityIDFromName("World");
		worldStatus = Gear::EntitySystem::GetStatus(worldID);

		WindowSelectorID = Gear::EntitySystem::GetEntityIDFromName("WindowSelector");
		WindowSelectorFSM = Gear::EntitySystem::GetFSM(WindowSelectorID);

		WindowSelectorRect.Right = 0.7f;
		WindowSelectorRect.Left = -0.7f;
		WindowSelectorRect.Top = 0.7f;
		WindowSelectorRect.Bottom = -0.7f;

		inFirst = false;
	}
	void MouseOnWindowSelectHandler::reset()
	{
		curTeamColor = std::any_cast<TeamColor::Color>(worldStatus->GetStat(WorldInfo::CurrentTeamColor));
		switch (curTeamColor)
		{
		case TeamColor::Red:
			virtualWindowSelectCursor = Gear::TextureStorage::GetAnimation2D("RedCursor");
			break;
		case TeamColor::Blue:
			virtualWindowSelectCursor = Gear::TextureStorage::GetAnimation2D("BlueCursor");
			break;
		case TeamColor::Green:
			virtualWindowSelectCursor = Gear::TextureStorage::GetAnimation2D("GreenCursor");
			break;
		case TeamColor::Yellow:
			virtualWindowSelectCursor = Gear::TextureStorage::GetAnimation2D("YellowCursor");
			break;
		case TeamColor::Purple:
			virtualWindowSelectCursor = Gear::TextureStorage::GetAnimation2D("PurpleCursor");
			break;
		case TeamColor::Sky:
			virtualWindowSelectCursor = Gear::TextureStorage::GetAnimation2D("SkyCursor");
			break;
		}

		virtualWindowSelectCursor->SetFrameY(12);

		mouseTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_VirtualWindowSelectPos.first, m_VirtualWindowSelectPos.second, ZOrder::z_FlatMouse))
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.16f, 0.16f, 1.0f));

		needReset = false;
	}
	Gear::EnumType MouseOnWindowSelectHandler::Handle(int entityID, const Gear::Command & cmd)
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
			m_VirtualWindowSelectPos.first += dx * MouseSensitiveX;
			m_VirtualWindowSelectPos.second += dy * MouseSensitiveY;
		}

		bool outRect = false;
		if (m_VirtualWindowSelectPos.first < WindowSelectorRect.Left)
		{
			m_VirtualWindowSelectPos.first = WindowSelectorRect.Left;
			outRect = true;
		}
		if (m_VirtualWindowSelectPos.first > WindowSelectorRect.Right)
		{
			m_VirtualWindowSelectPos.first = WindowSelectorRect.Right;
			outRect = true;
		}
		if (m_VirtualWindowSelectPos.second < WindowSelectorRect.Bottom)
		{
			m_VirtualWindowSelectPos.second = WindowSelectorRect.Bottom;
			outRect = true;
		}
		if (m_VirtualWindowSelectPos.second > WindowSelectorRect.Top)
		{
			m_VirtualWindowSelectPos.second = WindowSelectorRect.Top;
			outRect = true;
		}
		if (outRect)
		{
			Gear::EventSystem::DispatchEvent(EventChannel::MouseMove, Gear::EntityEvent(EventType::MouseMove, MouseMoveData(dx, dy)));
		}

		mouseTransform[3][0] = m_VirtualWindowSelectPos.first;
		mouseTransform[3][1] = m_VirtualWindowSelectPos.second;

		m_VirtualWindowSelectPickPoint.first = mouseTransform[3][0] - 0.02f;
		m_VirtualWindowSelectPickPoint.second = mouseTransform[3][1] + 0.02f;

		Gear::Renderer2D::DrawFixedAnimation(mouseTransform, virtualWindowSelectCursor);

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

		return WorldState::OnWindowSelect;
	}
}

