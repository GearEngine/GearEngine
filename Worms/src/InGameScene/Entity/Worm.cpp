#include "Worm.h"
#include "WormFSMHandler.h"

Worm::Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale)
{
	//Attach Component
	m_ID = Gear::EntitySystem::CreateEntity(true);
	m_EntityID = m_ID;


	Gear::EntitySystem::AttachComponent(m_ID, {
		Gear::ComponentID::Animantor, Gear::ComponentID::Drawer,
		Gear::ComponentID::Controller, Gear::ComponentID::Transform,
		Gear::ComponentID::Physics, Gear::ComponentID::SoundPlayer
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
		{ WormState::OnIdle, new OnIdleHandler}
	});

	Gear::EntitySystem::SetController(m_ID, {
		{ WormCommand::BackJump, GR_KEY_BACKSPACE}, { WormCommand::ChangeWorm, GR_KEY_TAB},
		{ WormCommand::Up, GR_KEY_UP},				{ WormCommand::Down, GR_KEY_DOWN},
		{ WormCommand::Left, GR_KEY_LEFT},			{ WormCommand::Right, GR_KEY_RIGHT},
		{ WormCommand::UseItem, GR_KEY_SPACE},		{ WormCommand::SetTimer1, GR_KEY_1},
		{ WormCommand::SetTimer2, GR_KEY_2},		{ WormCommand::SetTimer3, GR_KEY_3},
		{ WormCommand::SetTimer4, GR_KEY_4},		{ WormCommand::SetTimer5, GR_KEY_5}
	});
}

Worm::~Worm()
{
	Gear::EntitySystem::DeleteEntity(m_ID);
}


