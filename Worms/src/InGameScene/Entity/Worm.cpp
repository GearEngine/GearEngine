#include "Worm.h"
#include "WormFSMHandler.h"

Worm::Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale)
{
	//Attach Component
	AttatchAnimator();
	AttatchPhysics();
	AttatchSoundPlayer();
	AttatchFSM();
	AttatchTransform();
	AttatchController();
	AttatchDrawer();

	//Register Component specific
	//Animator
	m_Animator->AddAnimation("OnMove", Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnMove"), 0.05f, true));
	m_Animator->AddAnimation("OnUseItem", Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnUseItem"), 0.2f, true));
	m_Animator->AddAnimation("OnIdle", Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnIdle"), 0.2f, true));

	//Physics
	//TODO : Add Physics

	//Sound
	//TODO : Add Sound

	//TransForm
	m_Transform->SetTransform(position, rotation, scale);

	//FSM
	m_FSM->GetState().RegisterStates({
		{ WormState::OnMove, "OnMove"},						{ WormState::OnDye, "OnDye"}, 
		{ WormState::OnIdle, "OnIdle"},						{ WormState::OnReadyItemUse, "OnReadyItemUse"}, 
		{ WormState::OnUseItem, "OnUseItem"},				{ WormState::OnAir, "OnAir"}, 
		{ WormState::OnReadyJump, "OnReadyJump"},			{ WormState::OnAirItemUse, "OnAirItemUse"}, 
		{ WormState::OnTurnOver, "OnTurnOver"},				{ WormState::OnDamaged, "OnDamaged"},
	});
	m_FSM->RegisterHandler({
		{ WormState::OnMove, new OnMoveHandler},			{ WormState::OnDye, new OnDyeHandler}, 
		{ WormState::OnIdle, new OnIdleHandler},			{ WormState::OnReadyItemUse, new OnReadyItemUseHandler}, 
		{ WormState::OnUseItem, new OnUseItemHandler },		{ WormState::OnAir, new OnAirHandler },
		{ WormState::OnReadyJump, new OnReadyJumpHandler},	{ WormState::OnAirItemUse, new OnAirItemUseHandler},
		{ WormState::OnTurnOver, new OnTurnOverHandler},	{ WormState::OnDamaged, new OnDamagedHandler},
	});

	//Controller
	m_Controller->RegisterCommand({
		{ WormCommand::BackJump, GR_KEY_BACKSPACE}, { WormCommand::ChangeWorm, GR_KEY_TAB},
		{ WormCommand::Up, GR_KEY_UP},				{ WormCommand::Down, GR_KEY_DOWN},
		{ WormCommand::Left, GR_KEY_LEFT},			{ WormCommand::Right, GR_KEY_RIGHT},
		{ WormCommand::UseItem, GR_KEY_SPACE},		{ WormCommand::SetTimer1, GR_KEY_1},
		{ WormCommand::SetTimer2, GR_KEY_2},		{ WormCommand::SetTimer3, GR_KEY_3},
		{ WormCommand::SetTimer4, GR_KEY_4},		{ WormCommand::SetTimer5, GR_KEY_5}
	});

	//Drawer
	m_Drawer->LinkAnimation(m_Animator);
	m_Drawer->LinkTransform(m_Transform);

	m_Animator->PlayAnimation("OnMove");
}

void Worm::Update(Gear::Timestep ts)
{
	m_Controller->Update(ts);
	m_Animator->Update(ts);
}

void Worm::Render()
{
	m_Drawer->Render();
}
