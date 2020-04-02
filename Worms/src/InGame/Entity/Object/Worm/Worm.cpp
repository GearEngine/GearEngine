#include "wmpch.h"
#include "Worm.h"

#include "InGame/Entity/Object/Item/ItemEnum.h"
#include "WormPixelCollisionHandler.h"

namespace InGame {

	Gear::Ref<WormExplosionEventHandler> Worm::s_ExplosionEventHandler = Gear::CreateRef<WormExplosionEventHandler>();
	Gear::Ref<WormWorldEventHandler> Worm::s_WorldEventHandler = Gear::CreateRef<WormWorldEventHandler>();

	Worm::Worm(int teamNumber, int wormNumber, const InitiateData& initData)
	{
		auto teamData = initData.Teams[teamNumber];
		auto wormData = initData.Teams[teamNumber].worms[wormNumber];

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
		std::vector<std::pair<int, int>> jumpAniOrder;
		for (int i = 0; i < 7; ++i)
		{
			jumpAniOrder.push_back({ 0, 6 - i });
		}
		std::vector<std::pair<int, int>> landAniOrder;
		for(int i = 0; i < 6; ++i)
		{
			landAniOrder.push_back({ 0, 5 - i });
		}
		std::vector<std::pair<int, int>> flipAniOrder;
		for (int i = 0; i < 22; ++i)
		{
			flipAniOrder.push_back({ 0, 21 - i });
		}
		std::vector<std::pair<int, int>> stuckAniOrder;
		for (int i = 0; i < 49; ++i)
		{
			stuckAniOrder.push_back({ 0, 48 - i });
		}
		std::vector<std::pair<int, int>> afterSlidingAniOrder;
		for (int i = 0; i < 22; ++i)
		{
			afterSlidingAniOrder.push_back({ 0, 21 - i });
		}
		std::vector<std::pair<int, int>> afterDamagedSlidingAniOrder;
		for (int i = 0; i < 36; ++i)
		{
			afterDamagedSlidingAniOrder.push_back({ 0, 35 - i });
		}
		std::vector<std::pair<int, int>> slidingAniOder;
		for(int i = 0 ; i < 5; ++i)
		{
			if (i < 3)
			{
				slidingAniOder.push_back({ 0, i });
			}
			else
			{
				slidingAniOder.push_back({ 0, 4 - i });
			}
		}

		Gear::Ref<Gear::Animation2D> empty;
		Gear::EntitySystem::SetAnimator(m_ID, {
			{ WormState::OnNotMyTurn,		empty },
			{ WormState::OnTurnOver,		empty },

			{ WormState::OnRightFlatBreath, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBreath"), 0.02f, birthAniOrder, true) },
			{ WormState::OnRightUpBreath, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBreath"), 0.02f, birthAniOrder, true) },
			{ WormState::OnRightDownBreath, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBreath"), 0.02f, birthAniOrder, true) },
			{ WormState::OnLeftFlatBreath, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBreath"), 0.02f, birthAniOrder, true) },
			{ WormState::OnLeftUpBreath, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBreath"), 0.02f, birthAniOrder, true) },
			{ WormState::OnLeftDownBreath, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBreath"), 0.02f, birthAniOrder, true) },

			{ WormState::OnRightFlatMove,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatWalk"), 0.02f, true) },
			{ WormState::OnRightUpMove,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpWalk"), 0.02f, true) },
			{ WormState::OnRightDownMove,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownWalk"), 0.02f, true) },
			{ WormState::OnLeftFlatMove,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatWalk"), 0.02f, true) },
			{ WormState::OnLeftUpMove,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpWalk"), 0.02f, true) },
			{ WormState::OnLeftDownMove,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownWalk"), 0.02f, true) },

			{ WormState::OnRightJump, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightJump"), 0.1f, jumpAniOrder, false) },
			{ WormState::OnLeftJump, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftJump"), 0.1f, jumpAniOrder, false) },
			{ WormState::OnRightBackJump, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightJump"), 0.1f, false) },
			{ WormState::OnLeftBackJump, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftJump"), 0.1f, false) },
			{ WormState::OnRightBackFlip, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightBackFlip"), 0.017f, flipAniOrder, false) },
			{ WormState::OnLeftBackFlip, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftBackFlip"), 0.017f, flipAniOrder, false) },

			{ WormState::OnRightFlatJumpReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatReadyJump"), 0.03f, false)},
			{ WormState::OnRightUpJumpReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpReadyJump"), 0.03f, false)},
			{ WormState::OnRightDownJumpReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownReadyJump"), 0.03f, false)},
			{ WormState::OnLeftFlatJumpReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatReadyJump"), 0.03f, false)},
			{ WormState::OnLeftUpJumpReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpReadyJump"), 0.03f, false)},
			{ WormState::OnLeftDownJumpReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownReadyJump"), 0.03f, false)},

			{ WormState::OnRightFlatLand, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatLand"), 0.03f,landAniOrder, false)},
			{ WormState::OnRightUpLand, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpLand"), 0.03f,landAniOrder, false)},
			{ WormState::OnRightDownLand, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownLand"), 0.03f,landAniOrder, false)},
			{ WormState::OnLeftFlatLand, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatLand"), 0.03f,landAniOrder, false)},
			{ WormState::OnLeftUpLand, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpLand"), 0.03f,landAniOrder, false)},
			{ WormState::OnLeftDownLand, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownLand"), 0.03f,landAniOrder, false)},

			{ WormState::OnLeftFlatSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatSliding"), 0.02f, slidingAniOder, true)},
			{ WormState::OnLeftUpSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpSliding"), 0.02f, slidingAniOder, true)},
			{ WormState::OnLeftDownSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownSliding"), 0.02f, slidingAniOder, true)},
			{ WormState::OnRightFlatSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatSliding"), 0.02f, slidingAniOder, true)},
			{ WormState::OnRightUpSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpSliding"), 0.02f, slidingAniOder, true)},
			{ WormState::OnRightDownSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownSliding"), 0.02f, slidingAniOder,true)},

			{ WormState::OnLeftFlatAfterSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatAfterSliding"), 0.02f, afterSlidingAniOrder, false)},
			{ WormState::OnLeftUpAfterSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpAfterSliding"), 0.02f, afterSlidingAniOrder, false)},
			{ WormState::OnLeftDownAfterSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownAfterSliding"), 0.02f, afterSlidingAniOrder, false)},
			{ WormState::OnRightFlatAfterSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatAfterSliding"), 0.02f, afterSlidingAniOrder, false)},
			{ WormState::OnRightUpAfterSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpAfterSliding"), 0.02f, afterSlidingAniOrder, false)},
			{ WormState::OnRightDownAfterSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownAfterSliding"), 0.02f, afterSlidingAniOrder, false)},

			{ WormState::OnLeftFlatAfterDamagedSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatAfterDamagedSliding"), 0.02f, afterDamagedSlidingAniOrder, false)},
			{ WormState::OnLeftUpAfterDamagedSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpAfterDamagedSliding"), 0.02f, afterDamagedSlidingAniOrder, false)},
			{ WormState::OnLeftDownAfterDamagedSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownAfterDamagedSliding"), 0.02f, afterDamagedSlidingAniOrder,false)},
			{ WormState::OnRightFlatAfterDamagedSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatAfterDamagedSliding"), 0.02f,afterDamagedSlidingAniOrder, false)},
			{ WormState::OnRightUpAfterDamagedSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpAfterDamagedSliding"), 0.02f, afterDamagedSlidingAniOrder, false)},
			{ WormState::OnRightDownAfterDamagedSliding,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownAfterDamagedSliding"), 0.02f, afterDamagedSlidingAniOrder, false)},

			{ WormState::OnLeftFlatStuck,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatStuck"), 0.02f, stuckAniOrder, false)},
			{ WormState::OnLeftUpStuck,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpStuck"), 0.02f, stuckAniOrder, false)},
			{ WormState::OnLeftDownStuck,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownStuck"), 0.02f,stuckAniOrder, false)},
			{ WormState::OnRightFlatStuck,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatStuck"), 0.02f, stuckAniOrder, false)},
			{ WormState::OnRightUpStuck,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpStuck"), 0.02f, stuckAniOrder, false)},
			{ WormState::OnRightDownStuck,  Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownStuck"), 0.02f, stuckAniOrder, false)},

			{ WormState::OnUseItem,	Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("OnUseItem"), 0.02f, true)},
		});
		
		//Set Transform
		Gear::EntitySystem::SetTransform(m_ID, wormData.StartPosition, 0.0f, initData.WormScale);

		//Set FSM
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WormState::OnWaiting, new WormOnWaitingHandler }, { WormState::OnNotMyTurn, new WormOnNotMyTurnHandler },
			{ WormState::OnMove, new WormOnMoveHandler}, {WormState::OnBreath, new WormOnBreathHandler},			
			{ WormState::OnAir, new WormOnAirHandler }, { WormState::OnTurnOver, new WormOnTurnOverHandler },
			{ WormState::OnReadyJump, new WormOnReadyJumpHandler }, {WormState::OnReadyBackJump, new WormOnReadyBackJumpHandler},
			{ WormState::OnReadyLand, new WormOnReadyLandHandler }, { WormState::OnReadyFallen, new WormOnReadyFallenHandler},
			{ WormState::OnLand, new WormOnLandHandler },			{ WormState::OnSliding, new WormOnSlidingHandler },
			{ WormState::OnStuck, new WormOnStuckHandler },			{ WormState::OnStandUp, new WormOnStandUpHandler }
		});

		//Set Controller
		Gear::EntitySystem::SetController(m_ID, {
			{ WormCommand::BackJump, GR_KEY_BACKSPACE },{ WormCommand::ChangeWorm, GR_KEY_TAB},
			{WormCommand::Jump, GR_KEY_ENTER },
			{ WormCommand::Up, GR_KEY_UP },				{ WormCommand::Down, GR_KEY_DOWN },
			{ WormCommand::Left, GR_KEY_LEFT },			{ WormCommand::Right, GR_KEY_RIGHT},
			{ WormCommand::UseItem, GR_KEY_SPACE },		{ WormCommand::SetTimer1, GR_KEY_1},
			{ WormCommand::SetTimer2, GR_KEY_2 },		{ WormCommand::SetTimer3, GR_KEY_3},
			{ WormCommand::SetTimer4, GR_KEY_4 },		{ WormCommand::SetTimer5, GR_KEY_5},
		});
		Gear::EntitySystem::InActivateComponent(m_ID, { Gear::ComponentID::Controller });

		//Set physics
		Gear::EntitySystem::SetPhysics(m_ID, true, 0.7f, 0.8f, 0.3f, 0.3f);
		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		
		auto mask = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.MapName + "Mask");
		int width = mask->GetWidth();
		int height = mask->GetHeight();

		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height/ initData.MapReductionRatio , 1.0f });
		Gear::EntitySystem::SetPixelCollision( m_ID, { 255, 255, 255 }, mask, maskTranslate, {
			{ "OnAir", Gear::CreateRef<WormOnAirPCHandler>() },
			{ "Move", Gear::CreateRef<WormMovePCHandler>() },
			{ "Sliding", Gear::CreateRef<WormSlidingPCHandler>() },
		});
		
		physics->SetPixelCollisionHandler("OnAir");

		//Set status
		Gear::EntitySystem::SetStatus(m_ID, {
			{ WormInfo::Stat::Name, wormData.Name}, { WormInfo::Stat::TeamColor, teamData.TeamColor }, { WormInfo::Stat::TeamName, teamData.TeamName }, 
			{ WormInfo::Stat::Hp, wormData.Hp }, { WormInfo::Stat::FireAngleVector , glm::vec2(0.70710678f, 0.70710678f) }, 
			{ WormInfo::Stat::FirePower, 0.0f}, { WormInfo::Stat::SelectedItem, ItemName::Bazooka }, 
			{ WormInfo::Stat::NameBorderOffset, 1.26f }, { WormInfo::Stat::HpBorderOffset, 0.7f }, { WormInfo::Stat::ZRenderOffset, wormData.AdditionalZRenderOffset },
			{ WormInfo::Stat::Direction, wormData.Direction}, { WormInfo::Stat::MoveSpeed, initData.WormMoveSpeed } ,
			{ WormInfo::Stat::MyTurn, false }
		});

		Gear::EntitySystem::SetStatusHanlder(m_ID, {
			{ WormStatusHandleType::Display, Gear::CreateRef<WormDisplayHanlder>() }, 
			{ WormStatusHandleType::WaitingDisplay, Gear::CreateRef<WormWaitingDisplayHanlder>()},
			{ WormStatusHandleType::DisplayPosChange, Gear::CreateRef<WormChangeDisplayPosHanlder>()}
		});

		auto NameBorder = Gear::TextureStorage::GetTexture2D("WormNameBorder");
		float nameBorderWidth = NameBorder->GetWidth();
		float nameBorderHeight = NameBorder->GetHeight();
		float nameBorderWidthUnit = nameBorderWidth / (initData.MapReductionRatio * 3);
		float nameBorderHeightUnit = nameBorderHeight / (initData.MapReductionRatio * 3);

		auto hpBorder = Gear::TextureStorage::GetTexture2D("WormHpBorder");
		float hpBorderWidth = hpBorder->GetWidth();
		float hpBorderHeight = hpBorder->GetHeight();
		float hpBorderWidthUnit = hpBorderWidth / (initData.MapReductionRatio * 3);
		float hpBorderHeightUnit = hpBorderHeight / (initData.MapReductionRatio * 3);

		auto status = Gear::EntitySystem::GetStatus(m_ID);
		WormDenoteData denoteData(nameBorderWidthUnit, nameBorderHeightUnit, NameBorder, hpBorderWidthUnit, hpBorderHeightUnit, hpBorder);

		status->PushNeedHandleData(WormStatusHandleType::Display, Gear::Status::StatHandleData(denoteData));
		status->PushNeedHandleData(WormStatusHandleType::WaitingDisplay, Gear::Status::StatHandleData(denoteData, true));

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::Explosion, s_ExplosionEventHandler);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::World, s_WorldEventHandler);

		auto animator = Gear::EntitySystem::GetAnimator2D(m_ID);
		switch (wormData.Direction)
		{
		case WormInfo::DirectionType::LeftFlat:
			animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
			animator->PlayAnimation(WormState::OnLeftFlatBreath);
			break;
		case WormInfo::DirectionType::RightFlat:
			animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
			animator->PlayAnimation(WormState::OnRightFlatBreath);
			break;
		}
		
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

