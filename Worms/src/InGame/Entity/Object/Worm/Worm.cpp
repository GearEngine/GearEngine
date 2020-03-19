#include "Worm.h"
#include "WormFSMHandler.h"
#include "EventChannel.h"

namespace InGame {

	Gear::Ref<WormExplosionEventHandler> Worm::s_ExplosionHandler = Gear::CreateRef<WormExplosionEventHandler>();

	Worm::Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale, const InitiateData& initData)
	{
		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Animantor,	Gear::ComponentID::Drawer,
			Gear::ComponentID::Controller,	Gear::ComponentID::Transform,
			Gear::ComponentID::Physics,		Gear::ComponentID::SoundPlayer,
			Gear::ComponentID::FSM,			Gear::ComponentID::Timer
		});

		//Set Component specific
		Gear::EntitySystem::SetAnimator(m_ID, {
			{ WormState::OnMove,    Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnMove"), 0.02f, true)},
			{ WormState::OnUseItem, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnUseItem"), 0.02f, true)},
			{ WormState::OnIdle,    Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnIdle"), 0.02f, true)}
			});

		Gear::EntitySystem::SetTransform(m_ID, position, rotation, scale);

		Gear::EntitySystem::SetFSM(m_ID, {
			{ WormState::OnMove, new OnMoveHandler }, { WormState::OnUseItem, new OnUseItemHandler },
			{ WormState::OnIdle, new OnIdleHandler }
			});

		Gear::EntitySystem::SetController(m_ID, {
			{ WormCommand::BackJump, GR_KEY_BACKSPACE}, { WormCommand::ChangeWorm, GR_KEY_TAB},
			{ WormCommand::Up, GR_KEY_UP},				{ WormCommand::Down, GR_KEY_DOWN},
			{ WormCommand::Left, GR_KEY_LEFT},			{ WormCommand::Right, GR_KEY_RIGHT},
			{ WormCommand::UseItem, GR_KEY_SPACE},		{ WormCommand::SetTimer1, GR_KEY_1},
			{ WormCommand::SetTimer2, GR_KEY_2},		{ WormCommand::SetTimer3, GR_KEY_3},
			{ WormCommand::SetTimer4, GR_KEY_4},		{ WormCommand::SetTimer5, GR_KEY_5}
			});

		Gear::EntitySystem::SetPhysics(m_ID, true, 10.0f, 10.0f, 0.3f, 0.3f);
		
		auto mask = Gear::TextureStorage::GetTexture2D(initData.MapName + "Mask");
		int width = mask->GetWidth();
		int height = mask->GetHeight();
		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height/ initData.MapReductionRatio , 1.0f });

		Gear::EntitySystem::SetPixelCollision(
			m_ID, { 234, 236, 232 }, mask, maskTranslate,
			{ {-0.1f, -0.2f}, {0.1f, -0.2f}, {0.0f, -0.2f} }
		);

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::Explosion, s_ExplosionHandler);
	}

	Worm::~Worm()
	{
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void Worm::Update(Gear::Timestep ts)
	{
		/*if (Gear::Input::IsKeyPressd(GR_KEY_C))
		{
			auto event = Gear::CreateRef<Gear::EntityEvent>(EventType::Explosion, ExplosionData({1.0f, 1.0f}, 3.0f, 5.0f));
			Gear::EventSystem::DispatchEvent(EventChannel::Explosion, event);
		}*/
	}

}

