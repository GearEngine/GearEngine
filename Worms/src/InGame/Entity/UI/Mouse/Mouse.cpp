#include "wmpch.h"
#include "Mouse.h"

#include "InGame/Data/EventData.h"
#include "MouseFSMHandler.h"
#include "MouseEventHandler.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace InGame {

	Mouse::Mouse(const InitiateData & initData)
	{
		m_Window = initData.window;
		m_MiddlePosition.first = initData.WindowWidth / 2;
		m_MiddlePosition.second = initData.WindowHeight / 2;

		m_CurrentPosition = m_MiddlePosition;
		glfwSetCursorPos((GLFWwindow*)m_Window, m_MiddlePosition.first, m_MiddlePosition.second);

		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Animator,	Gear::ComponentID::Drawer,
			Gear::ComponentID::FSM,			Gear::ComponentID::Timer,
			Gear::ComponentID::SoundPlayer, Gear::ComponentID::Controller,
		});

		//Set Component specific
		auto& cursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Cursor"), 0.0f, true);
		Gear::EntitySystem::SetAnimator(m_ID, {
			{ GameState::OnItemWindow,  cursor },	{ GameState::OnQuitWindow,	cursor  },
			{ GameState::OnWindowSelect,cursor },	{ GameState::OnRunning,		nullptr },
			{ GameState::OnWormsDying, nullptr },	{ GameState::OnStart,		nullptr },
			{ GameState::OnPrepareRun, nullptr },
		});

		auto runningHandler = new MouseOnGameRunningHandler;
		Gear::EntitySystem::SetFSM(m_ID, {
			{ GameState::OnItemWindow,		new MouseOnItemWindowHandler },
			{ GameState::OnQuitWindow,		new MouseOnQuitWindowHandler  },
			{ GameState::OnWindowSelect,	new MouseOnWindowSelectHandler },
			{ GameState::OnRunning, runningHandler },	{ GameState::OnWormsDying, runningHandler },
			{ GameState::OnStart, runningHandler },		{ GameState::OnPrepareRun, runningHandler },
		});

		Gear::EntitySystem::SetController(m_ID, {
			{ MOUSE_CLICKTYPE, GR_MOUSE_BUTTON_LEFT}, { MOUSE_CLICKTYPE, GR_MOUSE_BUTTON_RIGHT},
			{ 1, MOUSEMOVE_COMMAND }
		});

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::World, Gear::CreateRef<MouseEventHandler>());
	}

	Mouse::~Mouse()
	{
	}

	void Mouse::OnUpdate(Gear::Timestep ts)
	{
		auto[x, y] = Gear::Input::GetMousePosition();

		if (x != m_MiddlePosition.first || y != m_MiddlePosition.second)
		{
			float dx = m_MiddlePosition.first - x;
			float dy = m_MiddlePosition.second - y;
			auto event = Gear::CreateRef<Gear::EntityEvent>(EventType::MouseMove, MouseMoveData(dx, dy));
			Gear::EventSystem::DispatchEvent(EventChannel::MouseMove, event);
		}

		glfwSetCursorPos((GLFWwindow*)m_Window, m_MiddlePosition.first, m_MiddlePosition.second);
	}

}