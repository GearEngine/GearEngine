#include "wmpch.h"
#include "Worm.h"

#include "WormPixelCollisionHandler.h"
#include "InGame/Layer/ObjectLayer.h"
#include "WormNetEventController.h"

namespace InGame {

	Worm::Worm(int teamNumber, int wormNumber, const InitiateData& initData)
	{
		auto teamData = initData.Teams[teamNumber];
		auto wormData = initData.Teams[teamNumber].worms[wormNumber];

		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true, teamData.TeamName + wormData.Name);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Animator,	Gear::ComponentID::Drawer,
			Gear::ComponentID::Transform,	Gear::ComponentID::Status,
			Gear::ComponentID::Physics,		Gear::ComponentID::SoundPlayer,
			Gear::ComponentID::FSM,			Gear::ComponentID::Timer
		});

		if (initData.GameMode == GameMode::NetWork)
		{
			Gear::EntitySystem::AttachComponent(m_ID, { Gear::ComponentID::NetController });
		}
		else
		{
			Gear::EntitySystem::AttachComponent(m_ID, { Gear::ComponentID::Controller });
		}

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
		std::vector<std::pair<int, int>> _48AniOrder;
		for (int i = 0; i < 48; ++i)
		{
			_48AniOrder.push_back({ 0, 47 - i });
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
		std::vector<std::pair<int, int>> bazukaReadyAniOrder;
		for(int i = 0 ; i < 7; ++i)
		{
			bazukaReadyAniOrder.push_back({ 0, 6 - i });
		}
		std::vector<std::pair<int, int>> GrenadeReadyAniOrder;
		for (int i = 0; i < 10; ++i)
		{
			GrenadeReadyAniOrder.push_back({ 0, 9 - i });
		}

		std::vector<std::pair<int, int>> dyingAniOrder;
		for (int i = 0; i < 60; ++i)
		{
			dyingAniOrder.push_back({ 0, 59 - i });
		}

		std::vector<std::pair<int, int>> skipReadyAniOrder;
		for (int i = 0; i < 5; ++i)
		{
			skipReadyAniOrder.push_back({ 0, 4 - i });
		}
		std::vector<std::pair<int, int>> skipOnAniOrder;
		for (int i = 0; i < 9; ++i)
		{
			skipOnAniOrder.push_back({ 0, 8 - i });
		}

		Gear::Ref<Gear::Animation2D> empty;
		auto drawn = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Drawn"), 0.03f, true);
		drawn->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
		Gear::EntitySystem::SetAnimator(m_ID, {
			{ WormState::OnNotMyTurn,		empty },
			{ WormState::OnTurnOver,		empty },
			{ WormState::OnUnderWater,		drawn },
			{ WormState::OnVictory,		Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Victory"), 0.02f, true) },

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

			{ WormState::OnLeftFlatBazukaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBazukaReady"), 0.02f, bazukaReadyAniOrder, false)},
			{ WormState::OnLeftUpBazukaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBazukaReady"), 0.02f, bazukaReadyAniOrder, false)},
			{ WormState::OnLeftDownBazukaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBazukaReady"), 0.02f, bazukaReadyAniOrder, false)},
			{ WormState::OnRightFlatBazukaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBazukaReady"), 0.02f, bazukaReadyAniOrder, false)},
			{ WormState::OnRightUpBazukaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBazukaReady"), 0.02f, bazukaReadyAniOrder, false)},
			{ WormState::OnRightDownBazukaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBazukaReady"), 0.02f, bazukaReadyAniOrder, false)},

			{ WormState::OnLeftFlatBazukaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBazukaOn"), 0.02f, false)},
			{ WormState::OnLeftUpBazukaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBazukaOn"), 0.02f, false)},
			{ WormState::OnLeftDownBazukaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBazukaOn"), 0.02f, false)},
			{ WormState::OnRightFlatBazukaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBazukaOn"), 0.02f, false)},
			{ WormState::OnRightUpBazukaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBazukaOn"), 0.02f, false)},
			{ WormState::OnRightDownBazukaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBazukaOn"), 0.02f, false)},

			{ WormState::OnLeftFlatBazukaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBazukaReady"), 0.02f, false)},
			{ WormState::OnLeftUpBazukaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBazukaReady"), 0.02f, false)},
			{ WormState::OnLeftDownBazukaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBazukaReady"), 0.02f, false)},
			{ WormState::OnRightFlatBazukaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBazukaReady"), 0.02f, false)},
			{ WormState::OnRightUpBazukaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBazukaReady"), 0.02f, false)},
			{ WormState::OnRightDownBazukaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBazukaReady"), 0.02f, false)},

			{ WormState::OnLeftFlatGrenadeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatGrenadeReady"), 0.02f, GrenadeReadyAniOrder, false)},
			{ WormState::OnLeftUpGrenadeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpGrenadeReady"), 0.02f, GrenadeReadyAniOrder, false)},
			{ WormState::OnLeftDownGrenadeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownGrenadeReady"), 0.02f, GrenadeReadyAniOrder, false)},
			{ WormState::OnRightFlatGrenadeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatGrenadeReady"), 0.02f, GrenadeReadyAniOrder, false)},
			{ WormState::OnRightUpGrenadeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpGrenadeReady"), 0.02f, GrenadeReadyAniOrder, false)},
			{ WormState::OnRightDownGrenadeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownGrenadeReady"), 0.02f, GrenadeReadyAniOrder, false)},

			{ WormState::OnLeftFlatGrenadeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatGrenadeOn"), 0.02f, false)},
			{ WormState::OnLeftUpGrenadeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpGrenadeOn"), 0.02f, false)},
			{ WormState::OnLeftDownGrenadeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownGrenadeOn"), 0.02f, false)},
			{ WormState::OnRightFlatGrenadeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatGrenadeOn"), 0.02f, false)},
			{ WormState::OnRightUpGrenadeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpGrenadeOn"), 0.02f, false)},
			{ WormState::OnRightDownGrenadeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownGrenadeOn"), 0.02f, false)},

			{ WormState::OnLeftFlatGrenadeWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatGrenadeReady"), 0.02f, false)},
			{ WormState::OnLeftUpGrenadeWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpGrenadeReady"), 0.02f, false)},
			{ WormState::OnLeftDownGrenadeWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownGrenadeReady"), 0.02f, false)},
			{ WormState::OnRightFlatGrenadeWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatGrenadeReady"), 0.02f, false)},
			{ WormState::OnRightUpGrenadeWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpGrenadeReady"), 0.02f, false)},
			{ WormState::OnRightDownGrenadeWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownGrenadeReady"), 0.02f, false)},

			{ WormState::OnLeftFlatBananaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBananaReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpBananaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBananaReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownBananaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBananaReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatBananaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBananaReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpBananaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBananaReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownBananaReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBananaReady"), 0.02f, GrenadeReadyAniOrder, false) },

			{ WormState::OnLeftFlatBananaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBananaOn"), 0.02f, false) },
			{ WormState::OnLeftUpBananaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBananaOn"), 0.02f, false) },
			{ WormState::OnLeftDownBananaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBananaOn"), 0.02f, false) },
			{ WormState::OnRightFlatBananaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBananaOn"), 0.02f, false) },
			{ WormState::OnRightUpBananaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBananaOn"), 0.02f, false) },
			{ WormState::OnRightDownBananaOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBananaOn"), 0.02f, false) },

			{ WormState::OnLeftFlatBananaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatBananaReady"), 0.02f, false) },
			{ WormState::OnLeftUpBananaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpBananaReady"), 0.02f, false) },
			{ WormState::OnLeftDownBananaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownBananaReady"), 0.02f, false) },
			{ WormState::OnRightFlatBananaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatBananaReady"), 0.02f, false) },
			{ WormState::OnRightUpBananaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpBananaReady"), 0.02f, false) },
			{ WormState::OnRightDownBananaWithdraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownBananaReady"), 0.02f, false) },

			{ WormState::OnLeftFlatSkipGoReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatSkipGoReady"), 0.02f, skipReadyAniOrder, false) },
			{ WormState::OnLeftUpSkipGoReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpSkipGoReady"), 0.02f, skipReadyAniOrder, false) },
			{ WormState::OnLeftDownSkipGoReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownSkipGoReady"), 0.02f, skipReadyAniOrder, false) },
			{ WormState::OnRightFlatSkipGoReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatSkipGoReady"), 0.02f, skipReadyAniOrder, false) },
			{ WormState::OnRightUpSkipGoReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpSkipGoReady"), 0.02f, skipReadyAniOrder, false) },
			{ WormState::OnRightDownSkipGoReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownSkipGoReady"), 0.02f, skipReadyAniOrder, false) },

			{ WormState::OnLeftFlatSkipGoOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatSkipGoOn"), 0.04f, skipOnAniOrder, true) },
			{ WormState::OnLeftUpSkipGoOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpSkipGoOn"), 0.04f, skipOnAniOrder, true) },
			{ WormState::OnLeftDownSkipGoOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownSkipGoOn"), 0.04f, skipOnAniOrder, true) },
			{ WormState::OnRightFlatSkipGoOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatSkipGoOn"), 0.04f, skipOnAniOrder, true) },
			{ WormState::OnRightUpSkipGoOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpSkipGoOn"), 0.04f, skipOnAniOrder, true) },
			{ WormState::OnRightDownSkipGoOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownSkipGoOn"), 0.04f, skipOnAniOrder, true) },

			{ WormState::OnLeftFlatSurrenderReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatSurrenderReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpSurrenderReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpSurrenderReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownSurrenderReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownSurrenderReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatSurrenderReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatSurrenderReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpSurrenderReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpSurrenderReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownSurrenderReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownSurrenderReady"), 0.02f, GrenadeReadyAniOrder, false) },

			{ WormState::OnLeftFlatSurrenderOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatSurrenderOn"), 0.02f, true) },
			{ WormState::OnLeftUpSurrenderOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpSurrenderOn"), 0.02f, true) },
			{ WormState::OnLeftDownSurrenderOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownSurrenderOn"), 0.02f, true) },
			{ WormState::OnRightFlatSurrenderOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatSurrenderOn"), 0.02f, true) },
			{ WormState::OnRightUpSurrenderOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpSurrenderOn"), 0.02f, true) },
			{ WormState::OnRightDownSurrenderOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownSurrenderOn"), 0.02f, true) },
			
			{ WormState::OnLeftFlatTeleportReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatTeleportReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpTeleportReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpTeleportReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownTeleportReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownTeleportReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatTeleportReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatTeleportReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpTeleportReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpTeleportReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownTeleportReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownTeleportReady"), 0.02f, GrenadeReadyAniOrder, false) },
			
			{ WormState::OnLeftFlatTeleportWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatTeleportReady"), 0.02f, false) },
			{ WormState::OnLeftUpTeleportWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpTeleportReady"), 0.02f, false) },
			{ WormState::OnLeftDownTeleportWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownTeleportReady"), 0.02f, false) },
			{ WormState::OnRightFlatTeleportWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatTeleportReady"), 0.02f, false) },
			{ WormState::OnRightUpTeleportWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpTeleportReady"), 0.02f, false) },
			{ WormState::OnRightDownTeleportWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownTeleportReady"), 0.02f, false) },

			{ WormState::OnLeftFlatTeleportUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatTeleportUse"), 0.02f, _48AniOrder, false) },
			{ WormState::OnLeftUpTeleportUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpTeleportUse"), 0.02f, _48AniOrder, false) },
			{ WormState::OnLeftDownTeleportUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownTeleportUse"), 0.02f, _48AniOrder, false) },
			{ WormState::OnRightFlatTeleportUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatTeleportUse"), 0.02f, _48AniOrder, false) },
			{ WormState::OnRightUpTeleportUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpTeleportUse"), 0.02f, _48AniOrder, false) },
			{ WormState::OnRightDownTeleportUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownTeleportUse"), 0.02f, _48AniOrder, false) },

			{ WormState::OnLeftFlatTeleportAfterUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatTeleportUse"), 0.02f, false) },
			{ WormState::OnLeftUpTeleportAfterUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpTeleportUse"), 0.02f, false) },
			{ WormState::OnLeftDownTeleportAfterUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownTeleportUse"), 0.02f, false) },
			{ WormState::OnRightFlatTeleportAfterUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatTeleportUse"), 0.02f, false) },
			{ WormState::OnRightUpTeleportAfterUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpTeleportUse"), 0.02f, false) },
			{ WormState::OnRightDownTeleportAfterUse, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownTeleportUse"), 0.02f, false) },

			{ WormState::OnLeftFlatTeleportOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatTeleportOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpTeleportOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpTeleportOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownTeleportOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownTeleportOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatTeleportOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatTeleportOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpTeleportOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpTeleportOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownTeleportOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownTeleportOn"), 0.02f, GrenadeReadyAniOrder, false) },

			{ WormState::OnLeftFlatShotgunOn1, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatShotgunOn1"), 0.02f, false) },
			{ WormState::OnLeftUpShotgunOn1, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpShotgunOn1"), 0.02f, false) },
			{ WormState::OnLeftDownShotgunOn1, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownShotgunOn1"), 0.02f, false) },
			{ WormState::OnRightFlatShotgunOn1, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatShotgunOn1"), 0.02f, false) },
			{ WormState::OnRightUpShotgunOn1, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpShotgunOn1"), 0.02f, false) },
			{ WormState::OnRightDownShotgunOn1, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownShotgunOn1"), 0.02f, false) },

			{ WormState::OnLeftFlatShotgunOn2, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatShotgunOn2"), 0.02f, false) },
			{ WormState::OnLeftUpShotgunOn2, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpShotgunOn2"), 0.02f, false) },
			{ WormState::OnLeftDownShotgunOn2, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownShotgunOn2"), 0.02f, false) },
			{ WormState::OnRightFlatShotgunOn2, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatShotgunOn2"), 0.02f, false) },
			{ WormState::OnRightUpShotgunOn2, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpShotgunOn2"), 0.02f, false) },
			{ WormState::OnRightDownShotgunOn2, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownShotgunOn2"), 0.02f, false) },

			{ WormState::OnLeftFlatShotgunReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatShotgunReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpShotgunReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpShotgunReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownShotgunReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownShotgunReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatShotgunReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatShotgunReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpShotgunReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpShotgunReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownShotgunReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownShotgunReady"), 0.02f, GrenadeReadyAniOrder, false) },

			{ WormState::OnLeftFlatShotgunWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatShotgunReady"), 0.02f, false) },
			{ WormState::OnLeftUpShotgunWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpShotgunReady"), 0.02f, false) },
			{ WormState::OnLeftDownShotgunWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownShotgunReady"), 0.02f, false) },
			{ WormState::OnRightFlatShotgunWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatShotgunReady"), 0.02f, false) },
			{ WormState::OnRightUpShotgunWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpShotgunReady"), 0.02f, false) },
			{ WormState::OnRightDownShotgunWithraw, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownShotgunReady"), 0.02f, false) },

			{ WormState::OnLeftFlatShotgunShot, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatShotgunShot"), 0.02f, false) },
			{ WormState::OnLeftUpShotgunShot, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpShotgunShot"), 0.02f, false) },
			{ WormState::OnLeftDownShotgunShot, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownShotgunShot"), 0.02f, false) },
			{ WormState::OnRightFlatShotgunShot, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatShotgunShot"), 0.02f, false) },
			{ WormState::OnRightUpShotgunShot, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpShotgunShot"), 0.02f, false) },
			{ WormState::OnRightDownShotgunShot, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownShotgunShot"), 0.02f, false) },
			
			{ WormState::OnLeftFlatAirStrikeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatAirStrikeReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpAirStrikeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpAirStrikeReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownAirStrikeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownAirStrikeReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatAirStrikeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatAirStrikeReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpAirStrikeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpAirStrikeReady"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownAirStrikeReady, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownAirStrikeReady"), 0.02f, GrenadeReadyAniOrder, false) },

			{ WormState::OnLeftFlatAirStrikeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftFlatAirStrikeOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftUpAirStrikeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftUpAirStrikeOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnLeftDownAirStrikeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftDownAirStrikeOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightFlatAirStrikeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightFlatAirStrikeOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightUpAirStrikeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightUpAirStrikeOn"), 0.02f, GrenadeReadyAniOrder, false) },
			{ WormState::OnRightDownAirStrikeOn, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightDownAirStrikeOn"), 0.02f, GrenadeReadyAniOrder, false) },

			{ WormState::OnLeftFly,	Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftWormFly"), 0.02f, false)},
			{ WormState::OnRightFly, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightWormFly"), 0.02f, false)},
			
			{ WormState::OnRightDying, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RightWormDying"), 0.02f, dyingAniOrder, false)},
			{ WormState::OnLeftDying, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("LeftWormDying"), 0.02f, dyingAniOrder, false)},

		});
		
		//Set Transform
		auto mask = initData.Mapinfo.Mask;
		int width = mask->GetWidth();
		int height = mask->GetHeight();

		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height / initData.MapReductionRatio , 1.0f });

		int fixedBottomPos;
		int wormOnTexturePositionX;
		InitWormPosition(wormData, wormNumber, teamNumber, mask, maskTranslate, fixedBottomPos, wormOnTexturePositionX);
		Gear::EntitySystem::SetTransform(m_ID, wormData.StartPosition, 0.0f, initData.WormScale);

		//Set FSM
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WormState::OnWaiting, new WormOnWaitingHandler }, { WormState::OnNotMyTurn, new WormOnNotMyTurnHandler },
			{ WormState::OnMove, new WormOnMoveHandler}, {WormState::OnBreath, new WormOnBreathHandler},			
			{ WormState::OnAir, new WormOnAirHandler }, { WormState::OnTurnOver, new WormOnTurnOverHandler },
			{ WormState::OnReadyJump, new WormOnReadyJumpHandler }, {WormState::OnReadyBackJump, new WormOnReadyBackJumpHandler},
			{ WormState::OnReadyLand, new WormOnReadyLandHandler }, { WormState::OnReadyFallen, new WormOnReadyFallenHandler},
			{ WormState::OnLand, new WormOnLandHandler },			{ WormState::OnSliding, new WormOnSlidingHandler },
			{ WormState::OnStuck, new WormOnStuckHandler },			{ WormState::OnStandUp, new WormOnStandUpHandler },
			{ WormState::OnUnderWater, new WormOnUnderWaterHandler },	{ WormState::OnDamaged, new WormOnDamagedHandler },
			{ WormState::OnAfterDamaged, new WormOnAfterDamaged }, { WormState::OnReadyItemUse, new WormOnReadyItemUseHandler },
			{ WormState::OnItemWithdraw, new WormOnItemWithdraw }, { WormState::OnUseItem, new WormOnUseItemHandler },
			{ WormState::OnAttacked, new WormOnAttackedHandler }, { WormState::OnDye, new WormOnDyeHandler },
			{ WormState::OnAfterUseItem, new WormOnAfterUseItem }, { WormState::OnUseShotgun, new WormOnUseShotGun },
			{ WormState::OnUseWindowPickItem, new WormOnUseWindowPickItem},
		});

		//Set Controller
		if (initData.GameMode == GameMode::NetWork)
		{
			Gear::EntitySystem::SetNetController(m_ID, {
				{ WormCommand::BackJump, GR_KEY_BACKSPACE },{ WormCommand::ChangeWorm, GR_KEY_TAB},
				{ WormCommand::Jump, GR_KEY_ENTER },
				{ WormCommand::Up, GR_KEY_UP },				{ WormCommand::Down, GR_KEY_DOWN },
				{ WormCommand::Left, GR_KEY_LEFT },			{ WormCommand::Right, GR_KEY_RIGHT},
				{ WormCommand::UseItem, GR_KEY_SPACE },		{ WormCommand::SetTimer1, GR_KEY_1},
				{ WormCommand::SetTimer2, GR_KEY_2 },		{ WormCommand::SetTimer3, GR_KEY_3},
				{ WormCommand::SetTimer4, GR_KEY_4 },		{ WormCommand::SetTimer5, GR_KEY_5},
			});
			Gear::EntitySystem::InActivateComponent(m_ID, { Gear::ComponentID::NetController });
			auto netController = Gear::EntitySystem::GetNetController(m_ID);
			netController->RegisterEventCotroller(Gear::CreateRef<WormNetEventController>());
			if (wormData.MyNetCharacter)
			{
				netController->pushAccepterbleEntity(m_ID);
			}
		}
		else
		{
			Gear::EntitySystem::SetController(m_ID, {
				{ WormCommand::BackJump, GR_KEY_BACKSPACE },{ WormCommand::ChangeWorm, GR_KEY_TAB},
				{ WormCommand::Jump, GR_KEY_ENTER },
				{ WormCommand::Up, GR_KEY_UP },				{ WormCommand::Down, GR_KEY_DOWN },
				{ WormCommand::Left, GR_KEY_LEFT },			{ WormCommand::Right, GR_KEY_RIGHT},
				{ WormCommand::UseItem, GR_KEY_SPACE },		{ WormCommand::SetTimer1, GR_KEY_1},
				{ WormCommand::SetTimer2, GR_KEY_2 },		{ WormCommand::SetTimer3, GR_KEY_3},
				{ WormCommand::SetTimer4, GR_KEY_4 },		{ WormCommand::SetTimer5, GR_KEY_5},
				});
			Gear::EntitySystem::InActivateComponent(m_ID, { Gear::ComponentID::Controller });
		}

		//Set physics
		Gear::EntitySystem::SetPhysics(m_ID, false, 0.7f, 0.8f, 0.3f, 0.3f);
		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		
		Gear::EntitySystem::SetPixelCollision(m_ID, { 255, 255, 255 }, mask, maskTranslate, {
			{ "OnAir", Gear::CreateRef<WormOnAirPCHandler>() },
			{ "Move", Gear::CreateRef<WormMovePCHandler>() },
			{ "Sliding", Gear::CreateRef<WormSlidingPCHandler>() },
			{ "UnderWater", Gear::CreateRef<WormOnUnderWater>() },
			{ "Nothing", Gear::CreateRef< WormOnNotPhysics>() },
		});
		
		physics->SetPixelCollisionHandler("Move");

		//Set status
		Gear::EntitySystem::SetStatus(m_ID, {
			{ WormInfo::Stat::Name, wormData.Name}, { WormInfo::Stat::TeamColor, teamData.TeamColor }, { WormInfo::Stat::TeamName, teamData.TeamName }, 
			{ WormInfo::Stat::Hp, wormData.Hp }, { WormInfo::Stat::FireAngle, 15.0f },
			{ WormInfo::Stat::FirePower, 0.0f}, { WormInfo::Stat::SelectedItem, ItemInfo::Number::Bazooka }, 
			{ WormInfo::Stat::NameBorderOffset, 1.36f }, { WormInfo::Stat::HpBorderOffset, 0.8f }, { WormInfo::Stat::ZRenderOffset, wormData.AdditionalZRenderOffset },
			{ WormInfo::Stat::Direction, wormData.Direction}, { WormInfo::Stat::MoveSpeed, initData.WormMoveSpeed },
			{ WormInfo::Stat::MyTurn, false }, { WormInfo::Stat::Damage, 0 }, {WormInfo::Stat::SelfDamage , 0},
			{ WormInfo::Stat::ItemExplosionTime, 3.0f }, { WormInfo::Stat::UsedItem, false }, {WormInfo::Stat::Surrendered, false },
			{ WormInfo::Stat::TurnPassed, false }, { WormInfo::Stat::Grave, wormData.GraveType }, { WormInfo::Stat::ShotgunUseCnt, 0 },
			{ WormInfo::Stat::WindowPickedPoint, glm::vec2(NONE_PICK, NONE_PICK)}
		});

		Gear::EntitySystem::SetStatusHanlder(m_ID, {
			{ WormStatusHandleType::Display, Gear::CreateRef<WormDisplayHanlder>() },
			{ WormStatusHandleType::WaitingDisplay, Gear::CreateRef<WormWaitingDisplayHanlder>() },
			{ WormStatusHandleType::DisplayPosChange, Gear::CreateRef<WormChangeDisplayPosHanlder>() },
			{ WormStatusHandleType::Damaged, Gear::CreateRef<WormGetDamageHanlder>() },
			{ WormStatusHandleType::DisplayDamage, Gear::CreateRef<WormDisplayDamageHanlder>() },
			{ WormStatusHandleType::DisplayAim, Gear::CreateRef<WormDisplayAimHandler>() },
			{ WormStatusHandleType::AfterUseItem, Gear::CreateRef<WormAfterUseItemHanlder>() },
			{ WormStatusHandleType::SkipGo, Gear::CreateRef<WormSkipGoHanlder>() },
			{ WormStatusHandleType::Surrender, Gear::CreateRef<WormSurrenderHanlder>() },
			{ WormStatusHandleType::Teleport, Gear::CreateRef<WormTeleportHandler>()}
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
		status->PushNeedHandleData(WormStatusHandleType::Damaged, Gear::Status::StatHandleData(denoteData, true));
		status->PushNeedHandleData(WormStatusHandleType::DisplayAim, Gear::Status::StatHandleData(0, true));

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::World, Gear::CreateRef<WormWorldEventHandler>());
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::Explosion, Gear::CreateRef<WormExplosionEventHandler>());

		GradientCheck(wormOnTexturePositionX, fixedBottomPos, mask, wormData.Direction);
		
		auto worldID = Gear::EntitySystem::GetEntityIDFromName("World");
		Gear::EventSystem::DispatchEventTo(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::CreatedWorm, 0, m_ID)), worldID);
	}

	Worm::~Worm()
	{
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void Worm::Update(Gear::Timestep ts)
	{

	}

	void Worm::InitWormPosition(WormSpecific& wormData, int wormNumber, int teamNumber, Gear::Ref<Gear::Texture2D> mask ,const glm::mat4& maskTranslate, int& fixedBottomPos, int& wormOnTexturePositionX)
	{
		auto coordManager = Gear::Coord2DManger::Get();
		float width = (float)mask->GetWidth();
		float height = (float)mask->GetHeight();

		glm::vec3 targetPxiel(255.0f, 255.0f, 255.0f);
		auto textureLocalPosition = coordManager->GetTextureLocalPosition_From_WorlPosition(glm::vec2(wormData.StartPosition.x, wormData.StartPosition.y), maskTranslate);
		wormOnTexturePositionX = textureLocalPosition.x * width;
		float wormOnTexturePositionY = textureLocalPosition.y * height;

		int basicYOffset = 10;
		while (1)
		{
			fixedBottomPos = (int)wormOnTexturePositionY - basicYOffset;
			if (fixedBottomPos < 0)
			{
				wormData.StartPosition.x = Gear::Util::GetRndFloatFromTo(-25.0f, 25.0f);
				textureLocalPosition = coordManager->GetTextureLocalPosition_From_WorlPosition(glm::vec2(wormData.StartPosition.x, wormData.StartPosition.y), maskTranslate);
				wormOnTexturePositionX = textureLocalPosition.x * width;
				basicYOffset = 8;
				continue;
			}

			auto pixel = coordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { (int)wormOnTexturePositionX, fixedBottomPos });

			bool adjacentCheck = true;
			if (pixel == targetPxiel)
			{
				for (int i = 1; i < 11; ++i)
				{
					fixedBottomPos = (int)wormOnTexturePositionY - (basicYOffset - i);
					pixel = coordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { (int)wormOnTexturePositionX, fixedBottomPos });

					if (pixel != targetPxiel)
					{
						float localY = (fixedBottomPos + 8) / height - 0.5f;
						wormData.StartPosition.y = maskTranslate[1][1] * localY + maskTranslate[3][1];
						return;
					}
				}
			}
			basicYOffset += 10;
		}
	}

	void Worm::GradientCheck(int midX, int bottomY, Gear::Ref<Gear::Texture2D> mask, WormInfo::DirectionType dir)
	{
		int xOffset = 4;
		int gradRange = 4;

		auto leftPixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { midX - xOffset, bottomY });
		auto rightPixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { midX + xOffset, bottomY });

		glm::vec3 m_TargetPixelColor(255.0f, 255.0f, 255.0f);
		int leftOffset = 0;
		if (leftPixel == m_TargetPixelColor)
		{
			for (int i = 0; i < gradRange; ++i)
			{
				++leftOffset;
				leftPixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { midX - xOffset, bottomY + leftOffset });
				if (leftPixel != m_TargetPixelColor)
				{
					--leftOffset;
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < gradRange; ++i)
			{
				--leftOffset;
				leftPixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { midX - xOffset , bottomY + leftOffset });
				if (leftPixel == m_TargetPixelColor)
				{
					break;
				}
			}
		}

		int rightOffset = 0;
		if (rightPixel == m_TargetPixelColor)
		{
			for (int i = 0; i < gradRange; ++i)
			{
				++rightOffset;
				rightPixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { midX + xOffset, bottomY + rightOffset });
				if (rightPixel != m_TargetPixelColor)
				{
					--rightOffset;
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < gradRange; ++i)
			{
				--rightOffset;
				rightPixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { midX + xOffset, bottomY + rightOffset });
				if (rightPixel == m_TargetPixelColor)
				{
					break;
				}
			}
		}
		auto animator = Gear::EntitySystem::GetAnimator2D(m_ID);
		//result
		if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
		{
			if (leftOffset == rightOffset)
			{
				animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
			}
			if (leftOffset > rightOffset)
			{
				animator->SetCurrentAnimation(WormState::OnLeftUpBreath);
			}
			if (leftOffset < rightOffset)
			{
				animator->SetCurrentAnimation(WormState::OnLeftDownBreath);
			}
		}
		else
		{
			if (leftOffset == rightOffset)
			{
				animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
			}
			if (leftOffset > rightOffset)
			{
				animator->SetCurrentAnimation(WormState::OnRightDownBreath);
			}
			if (leftOffset < rightOffset)
			{
				animator->SetCurrentAnimation(WormState::OnRightUpBreath);
			}
		}
		animator->ResumeAnimation();
	}
}

