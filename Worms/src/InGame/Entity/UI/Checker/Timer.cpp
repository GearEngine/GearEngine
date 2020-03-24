#include "wmpch.h"
#include "Timer.h"

#include "TimerEventHandler.h"

namespace InGame {

	Timer::Timer(const InitiateData& initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity(true);

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Transform, Gear::ComponentID::Drawer,
			Gear::ComponentID::Physics,   Gear::ComponentID::Texturer,
			Gear::ComponentID::Timer
		});

		auto timerBoarder = Gear::TextureStorage::GetTexture2D("TimerBorder");
		int width = timerBoarder->GetWidth();
		int height = timerBoarder->GetHeight() * initData.WindowAspectRatio;

		glm::vec3 position(-0.91f, -0.84f, ZOrder::z_Checker);
		glm::vec3 scale(width / 1700.0f, height / 1700.0f, 1.0f);
		Gear::EntitySystem::SetTransform(m_ID, position, 0.0f, scale);

		Gear::EntitySystem::SetPhysics(m_ID);

		Gear::EntitySystem::SetTexturer(m_ID, Gear::RenderType::Fixed, timerBoarder);
		
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::World, Gear::CreateRef<TimerEventHandler>());
	}

	Timer::~Timer()
	{
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

}