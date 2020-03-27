#include "wmpch.h"
#include "Worm.h"

#include "WormStatusHandler.h"
#include "InGame/Entity/Object/Item/ItemEnum.h"

namespace InGame {

	Gear::Ref<WormExplosionEventHandler> Worm::s_ExplosionHandler = Gear::CreateRef<WormExplosionEventHandler>();

	Worm::Worm(int count, const InitiateData& initData)
	{
		auto wormData = initData.Worms[count];

		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Animator,	Gear::ComponentID::Drawer,
			Gear::ComponentID::Controller,	Gear::ComponentID::Transform,
			Gear::ComponentID::Physics,		Gear::ComponentID::SoundPlayer,
			Gear::ComponentID::FSM,			Gear::ComponentID::Timer,
			Gear::ComponentID::Status
		});

		//Set Component specific
		//Set Animator
		std::vector<std::pair<int, int>> birthAniOrder;
		for (int i = 0; i < 40; ++i)
		{
			if (i < 20)
			{
				birthAniOrder.push_back({ 0, i });
			}
			else
			{
				birthAniOrder.push_back({ 0, 39 - i });
			}
		}
		Gear::EntitySystem::SetAnimator(m_ID, {
			{ WormState::OnMove,    Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnMove"), 0.02f, true)},
			{ WormState::OnUseItem, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnUseItem"), 0.02f, true)},
			{ WormState::OnIdle,    Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnIdle"), 0.04f, birthAniOrder, true)}
		});

		//Set Transform
		Gear::EntitySystem::SetTransform(m_ID, wormData.StartPosition, 0.0f, initData.WormScale);

		//Set FSM
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WormState::OnMove, new WormOnMoveHandler },	{ WormState::OnUseItem, new WormOnUseItemHandler },
			{ WormState::OnIdle, new WormOnIdleHandler },	{ WormState::OnTurnOver, new WormOnTurnOverHandler },
			{ WormState::OnReady, new WormOnReadyHandler }	
		});

		//Set Controller
		Gear::EntitySystem::SetController(m_ID, {
			{ WormCommand::BackJump, GR_KEY_BACKSPACE },{ WormCommand::ChangeWorm, GR_KEY_TAB},
			{ WormCommand::Up, GR_KEY_UP },				{ WormCommand::Down, GR_KEY_DOWN},
			{ WormCommand::Left, GR_KEY_LEFT },			{ WormCommand::Right, GR_KEY_RIGHT},
			{ WormCommand::UseItem, GR_KEY_SPACE },		{ WormCommand::SetTimer1, GR_KEY_1},
			{ WormCommand::SetTimer2, GR_KEY_2 },		{ WormCommand::SetTimer3, GR_KEY_3},
			{ WormCommand::SetTimer4, GR_KEY_4 },		{ WormCommand::SetTimer5, GR_KEY_5},
			{ WormCommand::ChangeWorm, GR_KEY_Z }
		});

		//Set physics
		Gear::EntitySystem::SetPhysics(m_ID, true, 10.0f, 10.0f, 0.3f, 0.3f);
		
		auto mask = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.MapName + "Mask");
		int width = mask->GetWidth();
		int height = mask->GetHeight();

		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height/ initData.MapReductionRatio , 1.0f });

		Gear::EntitySystem::SetPixelCollision(
			m_ID, { 255, 255, 255 }, mask, maskTranslate,
			{ {-0.1f, -0.2f}, {0.1f, -0.2f}, {0.0f, -0.2f} }
		);

		//Set status
		Gear::EntitySystem::SetStatus(m_ID, {
			{ WormInfo::Stat::Name, wormData.WormName}, { WormInfo::Stat::TeamColor, wormData.TeamColor }, { WormInfo::Stat::TeamName, wormData.TeamName }, 
			{ WormInfo::Stat::Hp, initData.WormHP }, { WormInfo::Stat::FireAngleVector , glm::vec2(0.70710678f, 0.70710678f) }, 
			{ WormInfo::Stat::FirePower, 0.0f}, { WormInfo::Stat::SelectedItem, ItemName::Bazooka }, 
			{ WormInfo::Stat::NameBorderOffset, 1.26f }, { WormInfo::Stat::HpBorderOffset, 0.7f }, { WormInfo::Stat::ZRenderOffset, wormData.AdditionalZRenderOffset }
		});

		Gear::EntitySystem::SetStatusHanlder(m_ID, {
			{ WormStatusHandleType::DisplayName, Gear::CreateRef<WormDisplayNameHanlder>() }, 
			{ WormStatusHandleType::DisplayHp, Gear::CreateRef<WormDisplayHpHanlder>() },
		});

		auto NameBorder = Gear::TextureStorage::GetTexture2D("WormNameBorder");
		float nameBorderWidth = NameBorder->GetWidth();
		float nameBorderHeight = NameBorder->GetHeight();
		float nameBorderWidthUnit = nameBorderWidth / (initData.MapReductionRatio * 3);
		float nameBorderHeightUnit = nameBorderHeight / (initData.MapReductionRatio * 3);
		auto status = Gear::EntitySystem::GetStatus(m_ID);
		status->PushNeedHandleData(WormStatusHandleType::DisplayName, 
			Gear::Status::StatHandleData(WormDenoteData(nameBorderWidthUnit, nameBorderHeightUnit, NameBorder)));
		
		auto hpBorder = Gear::TextureStorage::GetTexture2D("WormHpBorder");
		float hpBorderWidth = hpBorder->GetWidth();
		float hpBorderHeight = hpBorder->GetHeight();
		float hpBorderWidthUnit = hpBorderWidth / (initData.MapReductionRatio * 3);
		float hpBorderHeightUnit = hpBorderHeight / (initData.MapReductionRatio * 3);
		status->PushNeedHandleData(WormStatusHandleType::DisplayHp,
			Gear::Status::StatHandleData(WormDenoteData(hpBorderWidthUnit, hpBorderHeightUnit, hpBorder)));

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::Explosion, s_ExplosionHandler);

		Gear::EntitySystem::InActivateComponent(m_ID, { Gear::ComponentID::Physics });
	}

	Worm::~Worm()
	{
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void Worm::Update(Gear::Timestep ts)
	{

	}

}

