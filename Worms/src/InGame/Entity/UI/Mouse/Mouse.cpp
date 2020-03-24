#include "wmpch.h"
#include "Mouse.h"

#include "MouseFSMHandler.h"
#include "MouseEventHandler.h"

namespace InGame {

	Mouse::Mouse(const InitiateData & initData)
	{
		m_MiddlePosition.first = initData.WindowWidth / 2;
		m_MiddlePosition.second = initData.WindowHeight / 2;

		m_CurrentPosition = m_MiddlePosition;
		glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), m_MiddlePosition.first, m_MiddlePosition.second);

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
			{ WorldState::OnItemWindow,  cursor },	{ WorldState::OnQuitWindow,	cursor  },
			{ WorldState::OnWindowSelect,cursor },	{ WorldState::OnRunning,		nullptr },
			{ WorldState::OnWormsDying, nullptr },	{ WorldState::OnStart,		nullptr },
			{ WorldState::OnPrepareRun, nullptr },
		});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ WorldState::OnItemWindow,		new MouseOnItemWindowHandler },
			{ WorldState::OnQuitWindow,		new MouseOnQuitWindowHandler  },
			{ WorldState::OnWindowSelect,	new MouseOnWindowSelectHandler },
			{ WorldState::OnRunning,			new MouseOnGameRunningHandler },	
			{ WorldState::OnWormsDying,		new MouseOnGameRunningHandler },
			{ WorldState::OnStart,			new MouseOnGameRunningHandler },		
			{ WorldState::OnPrepareRun,		new MouseOnGameRunningHandler }
		});

		Gear::EntitySystem::SetController(m_ID, {
			{ MOUSE_CLICKTYPE, GR_MOUSE_BUTTON_LEFT}, { MOUSE_CLICKTYPE, GR_MOUSE_BUTTON_RIGHT},
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
	}

}