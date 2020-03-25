#include "wmpch.h"
#include "Timer.h"

#include "TimerFSMHandler.h"

namespace InGame {

	Timer::Timer(const InitiateData& initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(true);

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Transform, Gear::ComponentID::Drawer,
			Gear::ComponentID::Physics,   Gear::ComponentID::Texturer,
			Gear::ComponentID::Timer,	  Gear::ComponentID::FSM
		});

		auto timerBoarder = Gear::TextureStorage::GetTexture2D("TimerBorder");
		int width = timerBoarder->GetWidth();
		int height = timerBoarder->GetHeight() * initData.WindowAspectRatio;

		glm::vec3 scale(width / 1700.0f, height / 1700.0f, 1.0f);
		Gear::EntitySystem::SetTransform(m_ID, g_TimerDownPos, 0.0f, scale);

		Gear::EntitySystem::SetPhysics(m_ID);

		Gear::EntitySystem::SetTexturer(m_ID, Gear::RenderType::Fixed, timerBoarder);
		
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WorldState::OnRunning, new TimerOnRunningHandler }, 
			{ WorldState::OnPrepareRun, new TimerOnPrepareRunHandler },
			{ WorldState::OnPrepareNextPhase, new TimerOnPrepareNextPhaseHandler },
			{ WorldState::OnStart, new TimerOnStartHandler }
		});

		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::World, Gear::CreateRef<TimerEventHandler>());
	}

	Timer::~Timer()
	{
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

}