#include "WormEventHandle.h"

#include "InGame/Entity/Object/Effects/Effects.h"
#include "InGame/Entity/Object/Item/Item.h"
#include "InGame/Entity/World/WorldState.h"

namespace InGame {

	class WormOnMoveHandler : public Gear::FSM::InputHandler
	{
		bool isCompress = true;

		void WormOnMoveHandler::playWalkSound()
		{
			if (!IS_PLAYING_SOUND(WormsSound::wormWalk))
			{
				isCompress = !isCompress;
				if (isCompress)
				{
					PLAY_SOUND_NAME("Walk-Compress", WormsSound::wormWalk);
				}
				else
				{
					PLAY_SOUND_NAME("Walk-Expand", WormsSound::wormWalk);
				}
			}
		}


		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);

			if (GameMode::Bit::ModeBit == GameMode::Multi)
			{
				if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
				{
					return WormState::OnBreath;
				}
			}
			else
			{
				if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::NetController))
				{
					return WormState::OnBreath;
				}
			}


			if (cmd.KeyType == NONE_COMMAND || cmd.KeyType == WormCommand::Down || cmd.KeyType == WormCommand::Up)
			{
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				return WormState::OnBreath;
			}

			WormInfo::DirectionType dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Stat::Direction));

			auto curFrame = animator->GetFrameIdx();
			switch (dir)
			{
			case WormInfo::DirectionType::LeftFlat:
				animator->SetCurrentAnimation(WormState::OnLeftFlatMove);
				break;
			case WormInfo::DirectionType::LeftUp:
				animator->SetCurrentAnimation(WormState::OnLeftUpMove);
				break;
			case WormInfo::DirectionType::LeftDown:
				animator->SetCurrentAnimation(WormState::OnLeftDownMove);
				break;
			case WormInfo::DirectionType::RightFlat:
				animator->SetCurrentAnimation(WormState::OnRightFlatMove);
				break;
			case WormInfo::DirectionType::RightUp:
				animator->SetCurrentAnimation(WormState::OnRightUpMove);
				break;
			case WormInfo::DirectionType::RightDown:
				animator->SetCurrentAnimation(WormState::OnRightDownMove);
				break;
			}
			animator->ResumeAnimation();
			animator->SetFrameIdx(curFrame);

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::MoveSpeed));
			if (cmd.KeyType == WormCommand::Left)
			{
				playWalkSound();
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				playWalkSound();
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				isCompress = true;
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::BackJump)
			{
				isCompress = true;
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyBackJump;
			}

			return WormState::OnMove;
		}
	};

	class WormOnDyeHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		bool dyeSound = false;

		void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);

			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, entityID)));

			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (dir == WormInfo::LeftDown || dir == WormInfo::LeftFlat || dir == WormInfo::LeftUp)
			{
				animator->PlayAnimation(WormState::OnLeftDying);
			}
			else
			{
				animator->PlayAnimation(WormState::OnRightDying);
			}

			OnAwake = false;
		}

		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

	class WormOnAirHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static int yIndex = 0;
			static float pastTime = 0.0f;

			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto tick = Gear::EntitySystem::GetTimer(entityID)->GetTick();

			pastTime += tick;
			if (pastTime > 0.05f)
			{
				pastTime = 0.0f;
				if (yIndex)
				{
					yIndex = 0;
				}
				else
				{
					yIndex = 1;
				}
			}
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftUp || dir == WormInfo::DirectionType::LeftFlat)
			{
				animator->SetCurrentAnimation(WormState::OnLeftJump);
			}
			else
			{
				animator->SetCurrentAnimation(WormState::OnRightJump);
			}
			animator->SetFrameIdx({ 0, yIndex });

			return WormState::OnAir;
		}
	};

	inline void SetReadyItemUseAnimation(int entityID, WormInfo::DirectionType dir)
	{
		auto status = Gear::EntitySystem::GetStatus(entityID);
		auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
		ItemInfo::Number curSettedItem = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem));

		if (curSettedItem == ItemInfo::Number::Bazooka)
		{
			switch (dir)
			{
			case WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatBazukaReady);
				break;
			case WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatBazukaReady);
				break;
			case WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpBazukaReady);
				break;
			case WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpBazukaReady);
				break;
			case WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownBazukaReady);
				break;
			case WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownBazukaReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::Grenade)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatGrenadeReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatGrenadeReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpGrenadeReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpGrenadeReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownGrenadeReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownGrenadeReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::Banana)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatBananaReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatBananaReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpBananaReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpBananaReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownBananaReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownBananaReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::SkipGo)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatSkipGoReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatSkipGoReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpSkipGoReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpSkipGoReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownSkipGoReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownSkipGoReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::Surrender)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatSurrenderReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatSurrenderReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpSurrenderReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpSurrenderReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownSurrenderReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownSurrenderReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::Teleport)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatTeleportReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatTeleportReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpTeleportReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpTeleportReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownTeleportReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownTeleportReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::Donkey || curSettedItem == ItemInfo::Number::Hos)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatAirStrikeReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatAirStrikeReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpAirStrikeReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpAirStrikeReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownAirStrikeReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownAirStrikeReady);
				break;
			}
		}
		if (curSettedItem == ItemInfo::Number::Shotgun)
		{
			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatShotgunReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatShotgunReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpShotgunReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpShotgunReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownShotgunReady);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownShotgunReady);
				break;
			}
		}
	}

	class WormOnBreathHandler : public Gear::FSM::InputHandler
	{
		bool firstIn = true;
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);

			if (firstIn)
			{
				firstIn = false;
				timer->SetTimer(0.5f);
				timer->Start();
			}

			WormInfo::DirectionType dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Stat::Direction));
			switch (dir)
			{
			case WormInfo::DirectionType::LeftFlat:
				animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
				break;
			case WormInfo::DirectionType::LeftUp:
				animator->SetCurrentAnimation(WormState::OnLeftUpBreath);
				break;
			case WormInfo::DirectionType::LeftDown:
				animator->SetCurrentAnimation(WormState::OnLeftDownBreath);
				break;
			case WormInfo::DirectionType::RightFlat:
				animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
				break;
			case WormInfo::DirectionType::RightUp:
				animator->SetCurrentAnimation(WormState::OnRightUpBreath);
				break;
			case WormInfo::DirectionType::RightDown:
				animator->SetCurrentAnimation(WormState::OnRightDownBreath);
				break;
			}
			animator->ResumeAnimation();

			bool isItemUse = std::any_cast<bool>(status->GetStat(WormInfo::Stat::UsedItem));
			if (timer->isExpired() && !isItemUse)
			{
				SetReadyItemUseAnimation(entityID, dir);
				firstIn = true;
				return WormState::OnReadyItemUse;
			}

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::MoveSpeed));
			if (cmd.KeyType == WormCommand::Left)
			{
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
				firstIn = true;
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
				firstIn = true;
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				timer->SetTimer(0.2f);
				timer->Start();
				firstIn = true;
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::BackJump)
			{
				timer->SetTimer(0.2f);
				timer->Start();
				firstIn = true;
				return WormState::OnReadyBackJump;
			}

			return WormState::OnBreath;
		}
	};

	class WormOnReadyJumpHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static bool onFirst = true;
			static bool backJump = false;
			static glm::vec2 externalVector(3.4f, 4.0f);

			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (timer->isExpired())
			{
				if (cmd.KeyType == WormCommand::Jump)
				{
					backJump = true;
					if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
					{
						externalVector.x = 1.3f;
					}
					else
					{
						externalVector.x = -1.3f;
					}
				}
			}

			if (animator->loopCompleted())
			{
				physics->SetPixelCollisionHandler("OnAir");
				physics->ActivateGravity();
				physics->SetExternalVector(externalVector);

				if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					if (backJump)
					{
						animator->PlayAnimation(WormState::OnLeftBackJump);
					}
					else
					{
						animator->PlayAnimation(WormState::OnLeftJump);
					}
				}
				else
				{
					if (backJump)
					{
						animator->PlayAnimation(WormState::OnRightBackJump);
					}
					else
					{
						animator->PlayAnimation(WormState::OnRightJump);
					}

				}
				externalVector = { 3.4f, 4.0f };
				onFirst = true;
				backJump = false;
				PLAY_SOUND_NAME("JUMP" + std::to_string(Gear::Util::GetRndInt(2) + 1), WormsSound::wormSpeech);
				return WormState::OnJump;
			}

			if (onFirst)
			{
				onFirst = false;

				switch (dir)
				{
				case WormInfo::DirectionType::LeftFlat:
					externalVector.x = -externalVector.x;
					animator->PlayAnimation(WormState::OnLeftFlatJumpReady);
					break;
				case WormInfo::DirectionType::LeftUp:
					externalVector.x = -externalVector.x;
					animator->PlayAnimation(WormState::OnLeftUpJumpReady);
					break;
				case WormInfo::DirectionType::LeftDown:
					externalVector.x = -externalVector.x;
					animator->PlayAnimation(WormState::OnLeftDownJumpReady);
					break;
				case WormInfo::DirectionType::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatJumpReady);
					break;
				case WormInfo::DirectionType::RightUp:
					animator->PlayAnimation(WormState::OnRightUpJumpReady);
					break;
				case WormInfo::DirectionType::RightDown:
					animator->PlayAnimation(WormState::OnRightDownJumpReady);
					break;
				}
			}

			return WormState::OnReadyJump;
		}
	};

	class WormOnReadyBackJumpHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static bool onFirst = true;
			static bool backFlip = false;
			static glm::vec2 externalVector(0.0f, 0.0f);

			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (timer->isExpired())
			{
				if (cmd.KeyType == WormCommand::BackJump)
				{
					if (!backFlip)
					{
						backFlip = true;
						PLAY_SOUND_NAME("WORMSPRING", WormsSound::wormAct);
					}
					if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
					{
						externalVector.x = 0.7f;
					}
					else
					{
						externalVector.x = -0.7f;
					}
					externalVector.y = 6.0f;
				}
			}

			if (animator->loopCompleted())
			{
				physics->SetPixelCollisionHandler("OnAir");
				physics->ActivateGravity();
				physics->SetExternalVector(externalVector);

				if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					if (backFlip)
					{
						animator->PlayAnimation(WormState::OnLeftBackFlip);
					}
					else
					{
						animator->PlayAnimation(WormState::OnLeftBackJump);
					}
				}
				else
				{
					if (backFlip)
					{
						animator->PlayAnimation(WormState::OnRightBackFlip);
					}
					else
					{
						animator->PlayAnimation(WormState::OnRightBackJump);
					}
				}
				externalVector = { 0.0f, 0.0f };
				onFirst = true;
				if (!backFlip)
				{
					PLAY_SOUND_NAME("JUMP" + std::to_string(Gear::Util::GetRndInt(2) + 1), WormsSound::wormSpeech);
				}
				backFlip = false;
				return WormState::OnJump;
			}

			if (onFirst)
			{
				onFirst = false;
				externalVector.x = -1.3f;
				externalVector.y = 4.0f;

				switch (dir)
				{
				case WormInfo::DirectionType::LeftFlat:
					externalVector.x = -externalVector.x;
					animator->PlayAnimation(WormState::OnLeftFlatJumpReady);
					break;
				case WormInfo::DirectionType::LeftUp:
					externalVector.x = -externalVector.x;
					animator->PlayAnimation(WormState::OnLeftUpJumpReady);
					break;
				case WormInfo::DirectionType::LeftDown:
					externalVector.x = -externalVector.x;
					animator->PlayAnimation(WormState::OnLeftDownJumpReady);
					break;
				case WormInfo::DirectionType::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatJumpReady);
					break;
				case WormInfo::DirectionType::RightUp:
					animator->PlayAnimation(WormState::OnRightUpJumpReady);
					break;
				case WormInfo::DirectionType::RightDown:
					animator->PlayAnimation(WormState::OnRightDownJumpReady);
					break;
				}
			}
			return WormState::OnReadyBackJump;
		}
	};

	class WormOnLandHandler :public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			if (animator->loopCompleted())
			{
				auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
				auto status = Gear::EntitySystem::GetStatus(entityID);
				physics->SetPixelCollisionHandler("Move");

				if (!std::any_cast<bool>(status->GetStat(WormInfo::MyTurn)))
				{
					return WormState::OnNotMyTurn;
				}
				return WormState::OnBreath;
			}
			return WormState::OnLand;
		}
	};

	class WormOnReadyLandHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto& externalVector = physics->GetExternalVector();
			if (externalVector.y >= 0.0f)
			{
				physics->ResetGravityAccelation();
				externalVector.x = 0.0f;
				externalVector.y = 0.0f;
				return WormState::OnBreath;
			}
			else
			{
				physics->ResetGravityAccelation();
				externalVector.x = 0.0f;
				externalVector.y = 0.0f;

				auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatLand);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatLand);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpLand);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpLand);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownLand);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownLand);
					break;
				}
				PLAY_SOUND_NAME("WormLanding", WormsSound::wormAct);
				return WormState::OnLand;
			}
		}
	};

	class WormOnStandUpHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			if (animator->loopCompleted())
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				int selfDamage = std::any_cast<int>(status->GetStat(WormInfo::SelfDamage));
				int damage = std::any_cast<int>(status->GetStat(WormInfo::Damage));
				if (selfDamage + damage > 0)
				{
					return WormState::OnDamaged;
				}
				if (std::any_cast<bool>(status->GetStat(WormInfo::MyTurn)))
				{
					return WormState::OnBreath;
				}
				else
				{
					return WormState::OnNotMyTurn;
				}
			}
			return WormState::OnStandUp;
		}
	};

	class WormOnSlidingHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{

			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto& externalX = physics->GetExternalVectorX();
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (physics->GetPixelCollisionHandlerName() == "Sliding")
			{
				if (dir == WormInfo::DirectionType::RightDown || dir == WormInfo::DirectionType::LeftDown)
				{
					externalX *= 0.98f;
				}
				else if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::LeftUp)
				{
					externalX *= 0.92f;
				}
				else
				{
					externalX *= 0.95f;
				}

				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatSliding);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatSliding);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpSliding);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpSliding);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownSliding);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownSliding);
					break;
				}
				animator->ResumeAnimation();
			}

			if (std::abs(externalX) < 0.2f)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatAfterDamagedSliding);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatAfterDamagedSliding);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpAfterDamagedSliding);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpAfterDamagedSliding);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownAfterDamagedSliding);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownAfterDamagedSliding);
					break;
				}
				externalX = 0.0f;
				physics->SetPixelCollisionHandler("Move");
				return WormState::OnStandUp;
			}

			return WormState::OnSliding;
		}
	};

	class WormOnStuckHandler : public Gear::FSM::InputHandler
	{
		bool popSound = false;
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			GR_TRACE("FSM on Stuck Worm : {0}", entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			if (!popSound)
			{
				if (animator->GetFrameIdx().second < 20)
				{
					popSound = true;
					PLAY_SOUND_NAME("WORMPOP", WormsSound::wormAct);
				}
			}
			if (animator->loopCompleted())
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("Move");
				popSound = false;
				return WormState::OnDamaged;
			}
			return WormState::OnStuck;
		}
	};

	class WormOnReadyFallenHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			GR_TRACE("FSM on ReadyFallen Worm : {0}", entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);

			auto& externalVector = physics->GetExternalVector();
			float verticalRatio = externalVector.x / externalVector.y * 100.0f;
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			externalVector.y = 0.0f;
			physics->ResetGravityAccelation();

			if (std::abs(verticalRatio) < 8.0f)
			{
				PLAY_SOUND_NAME("TWANG" + std::to_string(Gear::Util::GetRndInt(6) + 1), WormsSound::wormAct);
				externalVector.x = 0.0f;
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatStuck);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatStuck);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpStuck);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpStuck);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownStuck);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownStuck);
					break;
				}
				status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
				return WormState::OnStuck;
			}
			else
			{
				PLAY_SOUND_NAME("OW" + std::to_string(Gear::Util::GetRndInt(3) + 1), WormsSound::wormAct);
				physics->SetPixelCollisionHandler("Sliding");
				status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
				return WormState::OnSliding;
			}

			return WormState::OnFallen;
		}
	};

	class WormOnDamagedHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{

			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);

			WormInfo::DirectionType dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Stat::Direction));
			switch (dir)
			{
			case WormInfo::DirectionType::LeftFlat:
				animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
				break;
			case WormInfo::DirectionType::LeftUp:
				animator->SetCurrentAnimation(WormState::OnLeftUpBreath);
				break;
			case WormInfo::DirectionType::LeftDown:
				animator->SetCurrentAnimation(WormState::OnLeftDownBreath);
				break;
			case WormInfo::DirectionType::RightFlat:
				animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
				break;
			case WormInfo::DirectionType::RightUp:
				animator->SetCurrentAnimation(WormState::OnRightUpBreath);
				break;
			case WormInfo::DirectionType::RightDown:
				animator->SetCurrentAnimation(WormState::OnRightDownBreath);
				break;
			}
			animator->ResumeAnimation();
			Gear::EventSystem::DispatchEventOnce(EventType::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::DamageWorm)));

			return WormState::OnNothing;
		}
	};

	class WormOnNothingHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnNothing;
		}
	};

	class WormOnReadyItemUseHandler : public Gear::FSM::InputHandler
	{
		bool inFirst = true;
		bool SettedAngle = false;
		bool afterReadyAni = false;
		float actualFireAngle;
		float firstFireAngle;
		int worldID;
		const float FixedFireAngle = 15.0f;
		bool initWindowSelect = false;

		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Status> worldStatus;
		Gear::Ref<Gear::FSM> worldFSM;
		std::string itemStr;
		bool canChangeTime = false;

		int timerCheckerID;
		ItemInfo::Number curItem;
		Gear::Ref<Gear::Status> timerStatus;

		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);

			timerCheckerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
			timerStatus = Gear::EntitySystem::GetStatus(timerCheckerID);
			worldID = Gear::EntitySystem::GetEntityIDFromName("World");
			worldStatus = Gear::EntitySystem::GetStatus(worldID);
			worldFSM = Gear::EntitySystem::GetFSM(worldID);
			OnAwake = false;
		}

		inline void OnOut(int entityID) override
		{
			inFirst = true;
			SettedAngle = false;
			afterReadyAni = false;
			initWindowSelect = false;
			status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
		}

		inline void SetWeaponOnAnimator(int entityID)
		{

			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (curItem == ItemInfo::Number::Bazooka)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatBazukaOn);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatBazukaOn);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpBazukaOn);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpBazukaOn);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownBazukaOn);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownBazukaOn);
					break;
				}
				animator->SetFrameY(int(31 - firstFireAngle));
				canChangeTime = false;
			}
			if (curItem == ItemInfo::Number::Grenade)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatGrenadeOn);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatGrenadeOn);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpGrenadeOn);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpGrenadeOn);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownGrenadeOn);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownGrenadeOn);
					break;
				}
				animator->SetFrameY(int(31 - firstFireAngle));
				itemStr = "Grenade";
				canChangeTime = true;
			}
			if (curItem == ItemInfo::Number::Banana)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatBananaOn);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatBananaOn);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpBananaOn);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpBananaOn);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownBananaOn);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownBananaOn);
					break;
				}
				animator->SetFrameY(int(31 - firstFireAngle));
				itemStr = "Banana";
				canChangeTime = true;
			}
			if (curItem == ItemInfo::Number::Shotgun)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatShotgunOn1);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatShotgunOn1);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpShotgunOn1);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpShotgunOn1);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownShotgunOn1);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownShotgunOn1);
					break;
				}
				animator->SetFrameY(int(31 - firstFireAngle));
				itemStr = "Banana";
				canChangeTime = false;
			}
			if (curItem == ItemInfo::Number::SkipGo)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatSkipGoOn);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatSkipGoOn);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpSkipGoOn);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpSkipGoOn);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownSkipGoOn);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownSkipGoOn);
					break;
				}
				animator->ResumeAnimation();
				canChangeTime = false;
			}
			if (curItem == ItemInfo::Number::Teleport)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatTeleportReady);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatTeleportReady);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpTeleportReady);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpTeleportReady);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownTeleportReady);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownTeleportReady);
					break;
				}
				animator->ResumeAnimation();
				canChangeTime = false;
			}
			if (curItem == ItemInfo::Number::Donkey || curItem == ItemInfo::Number::Hos)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatAirStrikeReady);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatAirStrikeReady);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpAirStrikeReady);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpAirStrikeReady);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownAirStrikeReady);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownAirStrikeReady);
					break;
				}
				animator->ResumeAnimation();
				canChangeTime = false;
			}
			if (curItem == ItemInfo::Number::Surrender)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatSurrenderOn);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatSurrenderOn);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpSurrenderOn);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpSurrenderOn);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownSurrenderOn);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownSurrenderOn);
					break;
				}
				animator->ResumeAnimation();
				canChangeTime = false;
			}
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			if (inFirst)
			{
				inFirst = false;
				actualFireAngle = (int)std::any_cast<float>(status->GetStat(WormInfo::FireAngle));

				curItem = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem));

				if (ItemInfo::isShowAim(curItem))
				{
					status->SetNeedHandleData(WormStatusHandleType::DisplayAim, false);
				}
				firstFireAngle = FixedFireAngle;
			}

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::Stat::MoveSpeed));

			if (animator->loopCompleted())
			{
				SetWeaponOnAnimator(entityID);
				afterReadyAni = true;
			}

			if (curItem < ItemInfo::Number::SkipGo)
			{
				if (curItem == ItemInfo::Teleport)
				{
					if (!initWindowSelect)
					{
						initWindowSelect = true;
						auto windowSelector = Gear::EntitySystem::GetEntityIDFromName("WindowSelector");
						Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, WindowSelectData(entityID, WindowSelctorType::Util, curItem)), windowSelector);
					}
					return WormState::OnReadyItemUse;
				}
				else if (curItem == ItemInfo::Donkey || curItem == ItemInfo::Hos)
				{
					if (!initWindowSelect)
					{
						initWindowSelect = true;
						auto windowSelector = Gear::EntitySystem::GetEntityIDFromName("WindowSelector");
						Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, WindowSelectData(entityID, WindowSelctorType::Drop, curItem)), windowSelector);
					}
					return WormState::OnReadyItemUse;
				}
				else
				{
					if (!SettedAngle && afterReadyAni)
					{
						if ((int)firstFireAngle != (int)actualFireAngle)
						{
							if (firstFireAngle > actualFireAngle)
							{
								if (std::abs(firstFireAngle - actualFireAngle) >= 2.0f)
								{
									firstFireAngle -= 2;
								}
								else
								{
									firstFireAngle -= 1;

								}
							}
							else
							{
								if (std::abs(actualFireAngle - firstFireAngle) >= 2.0f)
								{
									firstFireAngle += 2;
								}
								else
								{
									firstFireAngle += 1;
								}
							}
						}
						else
						{
							SettedAngle = true;
						}
						animator->SetFrameY((int)31 - firstFireAngle);
					}
				}
			}

			if (!afterReadyAni)
			{
				return WormState::OnReadyItemUse;
			}
			if (curItem == ItemInfo::Number::Shotgun && worldFSM->GetCurrentState() == WorldState::OnWaiting)
			{
				return WormState::OnReadyItemUse;
			}

			if (cmd.KeyType == WormCommand::BackJump)
			{
				timer->SetTimer(0.2f);
				timer->Start();
				OnOut(entityID);
				return WormState::OnReadyBackJump;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				timer->SetTimer(0.2f);
				timer->Start();
				OnOut(entityID);
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::Left)
			{
				physics->SetPixelCollisionHandler("Move");
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				OnOut(entityID);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetPixelCollisionHandler("Move");
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				OnOut(entityID);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::UseItem)
			{
				if (curItem == ItemInfo::Number::Bazooka)
				{
					timerStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
					PLAY_SOUND_NAME("WATCHTHIS", WormsSound::wormSpeech);
					PLAY_SOUND_NAME("ROCKETPOWERUP", WormsSound::wormAct);
					return WormState::OnUseItem;
				}
				else if (curItem == ItemInfo::Number::Shotgun)
				{
					timerStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
					PLAY_SOUND_NAME("SHOTGUNRELOAD", WormsSound::wormSpeech);
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
					return WormState::OnUseShotgun;
				}
				else if (curItem == ItemInfo::Number::Teleport)
				{

				}
				else if (curItem == ItemInfo::Number::Grenade || curItem == ItemInfo::Number::Banana)
				{
					timerStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
					PLAY_SOUND_NAME("FIRE", WormsSound::wormSpeech);
					PLAY_SOUND_NAME("THROWPOWERUP", WormsSound::wormAct);
					return WormState::OnUseItem;
				}
				else if (curItem == ItemInfo::Number::SkipGo)
				{
					PLAY_SOUND_NAME("COWARD", WormsSound::wormSpeech);
					timerStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
					status->PushNeedHandleData(WormStatusHandleType::SkipGo, Gear::Status::StatHandleData(0));
					return WormState::OnNothing;
				}
				else if (curItem == ItemInfo::Number::Surrender)
				{
					PLAY_SOUND_NAME("COWARD", WormsSound::wormSpeech);
					timerStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
					status->PushNeedHandleData(WormStatusHandleType::Surrender, Gear::Status::StatHandleData(0));

					return WormState::OnNothing;
				}
			}
			if (SettedAngle)
			{
				if (cmd.KeyType == WormCommand::Up)
				{
					firstFireAngle += 0.2f;

					if (firstFireAngle > 31.0f)
					{
						firstFireAngle = 31.0f;
					}

					animator->SetFrameY((int)31 - firstFireAngle);
					status->SetStat(WormInfo::FireAngle, firstFireAngle);
					return WormState::OnReadyItemUse;
				}
				if (cmd.KeyType == WormCommand::Down)
				{
					firstFireAngle -= 0.2f;

					if (firstFireAngle < 0.0f)
					{
						firstFireAngle = 0.0f;
					}

					animator->SetFrameY((int)31 - firstFireAngle);
					status->SetStat(WormInfo::FireAngle, firstFireAngle);
					return WormState::OnReadyItemUse;
				}
			}

			if (canChangeTime)
			{
				if (cmd.KeyType == WormCommand::SetTimer1)
				{
					status->SetStat(WormInfo::Stat::ItemExplosionTime, 1.0f);
					std::string str = itemStr + ", 1 sec, MIN Bounce";
					worldStatus->PopNeedHandleData(WorldStatusHandleType::DisplayMassage);
					worldStatus->PushNeedHandleData(WorldStatusHandleType::DisplayMassage, Gear::Status::StatHandleData(WorldMassage(str, FontType::White)));
				}
				if (cmd.KeyType == WormCommand::SetTimer2)
				{
					status->SetStat(WormInfo::Stat::ItemExplosionTime, 2.0f);
					std::string str = itemStr + ", 2 sec, MIN Bounce";
					worldStatus->PopNeedHandleData(WorldStatusHandleType::DisplayMassage);
					worldStatus->PushNeedHandleData(WorldStatusHandleType::DisplayMassage, Gear::Status::StatHandleData(WorldMassage(str, FontType::White)));
				}
				if (cmd.KeyType == WormCommand::SetTimer3)
				{
					status->SetStat(WormInfo::Stat::ItemExplosionTime, 3.0f);
					std::string str = itemStr + ", 3 sec, MIN Bounce";
					worldStatus->PopNeedHandleData(WorldStatusHandleType::DisplayMassage);
					worldStatus->PushNeedHandleData(WorldStatusHandleType::DisplayMassage, Gear::Status::StatHandleData(WorldMassage(str, FontType::White)));
				}
				if (cmd.KeyType == WormCommand::SetTimer4)
				{
					status->SetStat(WormInfo::Stat::ItemExplosionTime, 4.0f);
					std::string str = itemStr + ", 4 sec, MIN Bounce";
					worldStatus->PopNeedHandleData(WorldStatusHandleType::DisplayMassage);
					worldStatus->PushNeedHandleData(WorldStatusHandleType::DisplayMassage, Gear::Status::StatHandleData(WorldMassage(str, FontType::White)));
				}
				if (cmd.KeyType == WormCommand::SetTimer5)
				{
					status->SetStat(WormInfo::Stat::ItemExplosionTime, 5.0f);
					std::string str = itemStr + ", 5 sec, MIN Bounce";
					worldStatus->PopNeedHandleData(WorldStatusHandleType::DisplayMassage);
					worldStatus->PushNeedHandleData(WorldStatusHandleType::DisplayMassage, Gear::Status::StatHandleData(WorldMassage(str, FontType::White)));
				}
			}


			return WormState::OnReadyItemUse;
		}
	};

	class WormOnUseWindowPickItem : public Gear::FSM::InputHandler
	{
		bool awake = false;
		bool inFirst = true;

		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Status> Status;
		Gear::Ref<Gear::Animator2D> Animator;
		Gear::Ref<Gear::Physics2D> Physics;
		Gear::Ref<Gear::Transform2D> Transform;

		ItemInfo::Number selectedItem;

		int aniCompleteCount = 0;

		WormInfo::DirectionType dir;
		glm::vec3 pickedPoint;
		std::string teamName;

		virtual void Awake(int entityID) override
		{
			FSM = Gear::EntitySystem::GetFSM(entityID);
			Status = Gear::EntitySystem::GetStatus(entityID);
			Animator = Gear::EntitySystem::GetAnimator2D(entityID);
			Physics = Gear::EntitySystem::GetPhysics2D(entityID);
			Transform = Gear::EntitySystem::GetTransform2D(entityID);
		}

		void InFirst(int entityID)
		{
			inFirst = false;
			selectedItem = std::any_cast<ItemInfo::Number>(Status->GetStat(WormInfo::SelectedItem));
			dir = std::any_cast<WormInfo::DirectionType>(Status->GetStat(WormInfo::Direction));
			pickedPoint = glm::vec3(std::any_cast<glm::vec2>(Status->GetStat(WormInfo::WindowPickedPoint)), ZOrder::z_Worm);
			teamName = std::any_cast<std::string>(Status->GetStat(WormInfo::TeamName));
		}

		virtual void OnOut(int entityID) override
		{
			inFirst = true;
			aniCompleteCount = 0;
		}

		void SetUseAnimation(int mode)
		{
			if (mode == -1)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					Animator->PlayAnimation(WormState::OnLeftFlatBreath);
					break;
				case InGame::WormInfo::RightFlat:
					Animator->PlayAnimation(WormState::OnRightFlatBreath);
					break;
				case InGame::WormInfo::LeftUp:
					Animator->PlayAnimation(WormState::OnLeftUpBreath);
					break;
				case InGame::WormInfo::RightUp:
					Animator->PlayAnimation(WormState::OnRightUpBreath);
					break;
				case InGame::WormInfo::LeftDown:
					Animator->PlayAnimation(WormState::OnLeftDownBreath);
					break;
				case InGame::WormInfo::RightDown:
					Animator->PlayAnimation(WormState::OnRightDownBreath);
					break;
				}
				return;
			}
			if (selectedItem == ItemInfo::Teleport)
			{
				if (mode == 0)
				{
					switch (dir)
					{
					case InGame::WormInfo::LeftFlat:
						Animator->PlayAnimation(WormState::OnLeftFlatTeleportOn);
						break;
					case InGame::WormInfo::RightFlat:
						Animator->PlayAnimation(WormState::OnRightFlatTeleportOn);
						break;
					case InGame::WormInfo::LeftUp:
						Animator->PlayAnimation(WormState::OnLeftUpTeleportOn);
						break;
					case InGame::WormInfo::RightUp:
						Animator->PlayAnimation(WormState::OnRightUpTeleportOn);
						break;
					case InGame::WormInfo::LeftDown:
						Animator->PlayAnimation(WormState::OnLeftDownTeleportOn);
						break;
					case InGame::WormInfo::RightDown:
						Animator->PlayAnimation(WormState::OnRightDownTeleportOn);
						break;
					}
				}
				if (mode == 1)
				{
					switch (dir)
					{
					case InGame::WormInfo::LeftFlat:
						Animator->PlayAnimation(WormState::OnLeftFlatTeleportUse);
						break;
					case InGame::WormInfo::RightFlat:
						Animator->PlayAnimation(WormState::OnRightFlatTeleportUse);
						break;
					case InGame::WormInfo::LeftUp:
						Animator->PlayAnimation(WormState::OnLeftUpTeleportUse);
						break;
					case InGame::WormInfo::RightUp:
						Animator->PlayAnimation(WormState::OnRightUpTeleportUse);
						break;
					case InGame::WormInfo::LeftDown:
						Animator->PlayAnimation(WormState::OnLeftDownTeleportUse);
						break;
					case InGame::WormInfo::RightDown:
						Animator->PlayAnimation(WormState::OnRightDownTeleportUse);
						break;
					}
				}
				if (mode == 2)
				{
					switch (dir)
					{
					case InGame::WormInfo::LeftFlat:
						Animator->PlayAnimation(WormState::OnLeftFlatTeleportAfterUse);
						break;
					case InGame::WormInfo::RightFlat:
						Animator->PlayAnimation(WormState::OnRightFlatTeleportAfterUse);
						break;
					case InGame::WormInfo::LeftUp:
						Animator->PlayAnimation(WormState::OnLeftUpTeleportAfterUse);
						break;
					case InGame::WormInfo::RightUp:
						Animator->PlayAnimation(WormState::OnRightUpTeleportAfterUse);
						break;
					case InGame::WormInfo::LeftDown:
						Animator->PlayAnimation(WormState::OnLeftDownTeleportAfterUse);
						break;
					case InGame::WormInfo::RightDown:
						Animator->PlayAnimation(WormState::OnRightDownTeleportAfterUse);
						break;
					}
				}
			}
			if (selectedItem == ItemInfo::Donkey || selectedItem == ItemInfo::Hos)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					Animator->PlayAnimation(WormState::OnLeftFlatAirStrikeOn);
					break;
				case InGame::WormInfo::RightFlat:
					Animator->PlayAnimation(WormState::OnRightFlatAirStrikeOn);
					break;
				case InGame::WormInfo::LeftUp:
					Animator->PlayAnimation(WormState::OnLeftUpAirStrikeOn);
					break;
				case InGame::WormInfo::RightUp:
					Animator->PlayAnimation(WormState::OnRightUpAirStrikeOn);
					break;
				case InGame::WormInfo::LeftDown:
					Animator->PlayAnimation(WormState::OnLeftDownAirStrikeOn);
					break;
				case InGame::WormInfo::RightDown:
					Animator->PlayAnimation(WormState::OnRightDownAirStrikeOn);
					break;
				}
			}
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (!awake)
			{
				Awake(entityID);
			}
			if (inFirst)
			{
				InFirst(entityID);
			}
			
			if (selectedItem == ItemInfo::Teleport)
			{
				if (aniCompleteCount == 0)
				{
					SetUseAnimation(aniCompleteCount);
					++aniCompleteCount;
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
					return WormState::OnUseWindowPickItem;
				}
				if (aniCompleteCount == 1 && Animator->loopCompleted())
				{
					PLAY_SOUND_NAME("TELEPORT", WormsSound::Weapon);
					SetUseAnimation(aniCompleteCount);
					++aniCompleteCount;
					return WormState::OnUseWindowPickItem;
				}
				if (aniCompleteCount == 2 && Animator->loopCompleted())
				{
					PLAY_SOUND_NAME("TELEPORT", WormsSound::Weapon);
					SetUseAnimation(aniCompleteCount);
					++aniCompleteCount;
					Transform->SetPosition(pickedPoint);
					Physics->SetPixelCollisionHandler("Nothing");
					return WormState::OnUseWindowPickItem;
				}
				if (aniCompleteCount == 3)
				{
					if (Animator->loopCompleted())
					{
						Physics->SetPixelCollisionHandler("OnAir"); 
						Physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
						Physics->ActivateGravity();

						Status->SetStat(WormInfo::SelectedItem, ItemInfo::Bazooka);
						int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
						Gear::EntitySystem::GetStatus(timerID)->PushNeedHandleData(1, Gear::Status::StatHandleData(0));
						Status->PushNeedHandleData(WormStatusHandleType::Teleport, Gear::Status::StatHandleData(0));
						int ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
						Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(ItemInfo::Teleport, teamName)), ItemSelectorID);
						OnOut(entityID);
						return WormState::OnAir;
					}
				}
			}
			if (selectedItem == ItemInfo::Donkey)
			{
				if (aniCompleteCount == 0)
				{
					PLAY_SOUND_NAME("HOLYDONKEY", WormsSound::wormSpeech);
					SetUseAnimation(aniCompleteCount);
					++aniCompleteCount;
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
					return WormState::OnUseWindowPickItem;
				}
				if (aniCompleteCount == 1)
				{
					if (Animator->loopCompleted())
					{
						SetUseAnimation(-1);
						FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
						
						auto position = std::any_cast<glm::vec2>(Status->GetStat(WormInfo::WindowPickedPoint));
						auto item = ITEM_POOL->GetItem(ItemInfo::Donkey);
						item->init(glm::vec3(position, ZOrder::z_Item), 0.0f, 0.0f, entityID, 0.0f);

						Status->RegisterPushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
						Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, item->GetID())));
						int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
						Gear::EntitySystem::GetStatus(timerID)->PushNeedHandleData(1, Gear::Status::StatHandleData(0));
						int ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
						Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(ItemInfo::Donkey, teamName)), ItemSelectorID);
						Status->SetStat(WormInfo::MyTurn, false);
						OnOut(entityID);
						return WormState::OnNotMyTurn;
					}
				}
			}
			if (selectedItem == ItemInfo::Hos)
			{
				if (aniCompleteCount == 0)
				{
					PLAY_SOUND_NAME("YOUMUSTHAVEGONGMUL", WormsSound::Hos);
					STOP_SOUND_CAHNNEL(WormsSound::bgm);
					SetUseAnimation(aniCompleteCount);
					++aniCompleteCount;
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
					return WormState::OnUseWindowPickItem;
				}
				if (aniCompleteCount == 1)
				{
					if (Animator->loopCompleted())
					{
						++aniCompleteCount;
						SetUseAnimation(-1);
					}
				}
				if (aniCompleteCount == 2)
				{
					if (!IS_PLAYING_SOUND(WormsSound::Hos))
					{
						PLAY_SOUND_NAME("HOSTheme", WormsSound::bgm);
						Gear::SoundSystem::Get()->SetVolue(WormsSound::bgm, 1.0f);
						FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);

						auto position = std::any_cast<glm::vec2>(Status->GetStat(WormInfo::WindowPickedPoint));
						auto item = ITEM_POOL->GetItem(ItemInfo::Hos);
						item->init(glm::vec3(position, ZOrder::z_Item), 0.0f, 0.0f, entityID, 0.0f);

						Status->RegisterPushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
						Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, item->GetID())));
						int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
						Gear::EntitySystem::GetStatus(timerID)->PushNeedHandleData(1, Gear::Status::StatHandleData(0));
						int ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
						Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(ItemInfo::Hos, teamName)), ItemSelectorID);
						Status->SetStat(WormInfo::MyTurn, false);
						OnOut(entityID);
						return WormState::OnNotMyTurn;
					}
				}
			}
			return WormState::OnUseWindowPickItem;
		}
	};

	class WormOnUseShotGun : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Timer> timer;

		bool inFirst = true;
		float pastTime = 0.0f;
		const float pullTime = 0.4f;
		const float fireTime = 0.8f;
		const float shotCompleteTime = 1.1f;

		bool pullComplete = false;
		bool fireComplete = false;
		bool shotComplete = false;
		glm::vec2 noneHitPosition;
		static glm::vec2 hitPosition;
		static void calcShotPoint(Gear::Ref<Gear::Texture2D> mask, const glm::mat4& textureTrasform, const glm::vec3& position, float angle);

		inline void Awake(int entityID) override
		{
			noneHitPosition.x = 1000.0f;
			noneHitPosition.y = 0.0f;
			
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
			OnAwake = false;
		}

		void Shot(int entityID);

		inline void SetAnimator(int mode)
		{
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
			if (mode == 0)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatShotgunOn2);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatShotgunOn2);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpShotgunOn2);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpShotgunOn2);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownShotgunOn2);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownShotgunOn2);
					break;
				}
			}
			if (mode == 1)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatShotgunOn1);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatShotgunOn1);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpShotgunOn1);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpShotgunOn1);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownShotgunOn1);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownShotgunOn2);
					break;
				}
			}
			if (mode == 2)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatShotgunShot);
					break;
				case InGame::WormInfo::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatShotgunShot);
					break;
				case InGame::WormInfo::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpShotgunShot);
					break;
				case InGame::WormInfo::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpShotgunShot);
					break;
				case InGame::WormInfo::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownShotgunShot);
					break;
				case InGame::WormInfo::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownShotgunShot);
					break;
				}
			}
			if (mode == 3)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatShotgunWithraw);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatShotgunWithraw);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpShotgunWithraw);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpShotgunWithraw);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownShotgunWithraw);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownShotgunWithraw);
					break;
				}
				return;
			}
			float fireAngle = std::any_cast<float>(status->GetStat(WormInfo::FireAngle));
			animator->SetFrameY(31 - (int)fireAngle);
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			int useCount = std::any_cast<int>(status->GetStat(WormInfo::ShotgunUseCnt));
			if (inFirst)
			{
				hitPosition = noneHitPosition;
				inFirst = false;
				pullComplete = false;
				fireComplete = false;
				shotComplete = false;
				pastTime = 0.0f;

				auto terrain = Gear::EntitySystem::GetEntityIDFromName("Terrain");
				auto mask = Gear::EntitySystem::GetTexturer(terrain)->GetMask();
				auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
				auto nativeFireAngle = std::any_cast<float>(status->GetStat(WormInfo::FireAngle));
				auto position = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
				auto textureTransform = Gear::EntitySystem::GetPhysics2D(entityID)->GetPixelCollisionTargetTextureTranslate();
				float angle;
				if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					angle = 270.0f - nativeFireAngle / 31.0f * 180.0f;
				}
				else
				{
					angle = (nativeFireAngle - 15.5f) / 15.5f * 90.0f;
				}
				std::thread calcThread(WormOnUseShotGun::calcShotPoint, mask, textureTransform, position, angle);
				calcThread.detach();

				++useCount;
				status->SetStat(WormInfo::ShotgunUseCnt, useCount);
				SetAnimator(0);
			}

			pastTime += timer->GetTick();

			if (pastTime >= pullTime && !pullComplete)
			{
				SetAnimator(1);
				pullComplete = true;
			}
			if (pastTime >= fireTime && !fireComplete)
			{
				SetAnimator(2);
				Shot(entityID);
				PLAY_SOUND_NAME("ShotGunFire", WormsSound::Weapon);
				fireComplete = true;
			}
			if (pastTime >= shotCompleteTime && !shotComplete)
			{
				shotComplete = true;
			}

			if (shotComplete)
			{
				inFirst = true;
				if (useCount == 1)
				{
					SetAnimator(1);
					return WormState::OnReadyItemUse;
				}
				if (useCount == 2)
				{
					SetAnimator(3);
					auto teamName = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));
					Gear::EntitySystem::GetFSM(entityID)->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
					status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
					status->SetStat(WormInfo::ShotgunUseCnt, 0);
					int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
					int ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
					Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(ItemInfo::Shotgun, teamName)), ItemSelectorID);
					Gear::EntitySystem::GetStatus(timerID)->PushNeedHandleData(1, Gear::Status::StatHandleData(0));
					return WormState::OnItemWithdraw;
				}
			}
			return WormState::OnUseShotgun;
		}
	};

	class WormOnItemWithdraw : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Timer> timer;

		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			if (animator->loopCompleted())
			{
				auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatBreath);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatBreath);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpBreath);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpBreath);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownBreath);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownBreath);
					break;
				}
				timer->SetTimer(3.0f);
				timer->Start();
				return WormState::OnAfterUseItem;
			}
			return WormState::OnItemWithdraw;
		}
	};

	class WormOnAfterUseItem : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Status> status;
		bool inFirst = true;

		inline void OnOut(int entityID) override
		{
			inFirst = true;
		}

		inline void Awake(int entityID) override
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);

			OnAwake = false;
		}

		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			bool isUsedItem = std::any_cast<bool>(status->GetStat(WormInfo::Stat::UsedItem));
			if (!isUsedItem)
			{
				status->SetStat(WormInfo::Stat::UsedItem, true);
				auto itemExplosionTime = std::any_cast<float>(status->GetStat(WormInfo::Stat::ItemExplosionTime));
				ItemInfo::Number curItem = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::Stat::SelectedItem));

				switch (curItem)
				{
				case ItemInfo::Number::Bazooka:
					status->PushNeedHandleData(WormStatusHandleType::AfterUseItem, Gear::Status::StatHandleData(std::make_pair(2.0f, 2.1f)));
					break;
				case ItemInfo::Number::Grenade:
					status->PushNeedHandleData(WormStatusHandleType::AfterUseItem, Gear::Status::StatHandleData(std::make_pair(2.0f, itemExplosionTime + 0.5f)));
					break;
				case ItemInfo::Number::Banana:
					status->PushNeedHandleData(WormStatusHandleType::AfterUseItem, Gear::Status::StatHandleData(std::make_pair(2.0f, itemExplosionTime + 1.0f)));
					break;
				case ItemInfo::Number::Shotgun:
					status->PushNeedHandleData(WormStatusHandleType::AfterUseItem, Gear::Status::StatHandleData(std::make_pair(2.0f, 2.1f)));
				}
				return WormState::OnBreath;
			}
			else
			{
				if (std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem)) == ItemInfo::Shotgun)
				{
					status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
				}
				return WormState::OnNothing;
			}
		}
	};

	class WormOnAttackedHandler : public Gear::FSM::InputHandler
	{
		bool InFirst = true;

		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Timer> timer;

		float pastTime = 0.0f;
		const float FrameDelay = 0.04f;

		int textureIndexY;
		int textureIndexX = 0;

		void init(int entityID)
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
			InFirst = false;
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (InFirst)
			{
				init(entityID);
			}

			auto externalVector = physics->GetExternalVector();
			auto angle = Gear::Util::GetAngleFromXY(externalVector.x, externalVector.y);
			auto tick = timer->GetTick();

			if (externalVector.x < 0.0f)
			{
				angle = angle - 90.0f;
				textureIndexY = 31.0f - angle / 180.0f * 31.0f;
				animator->SetCurrentAnimation(WormState::OnLeftFly);
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
			}
			else
			{
				if (angle >= 0.0f && angle <= 90.0f)
				{
					angle = 90.0f - angle;
				}
				else
				{
					angle = 450.0f - angle;
				}
				textureIndexY = 31.0f - angle / 180.0f * 31.0f;
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
				animator->SetCurrentAnimation(WormState::OnRightFly);
			}

			pastTime += tick;
			if (pastTime > FrameDelay)
			{
				++textureIndexX;
				pastTime = 0.0f;
				if (textureIndexX >= 3)
				{
					textureIndexX = 0;
				}
			}

			animator->SetFrameIdx({ textureIndexX, textureIndexY });

			return WormState::OnAttacked;
		}
	};

	class WormOnUseItemHandler : public Gear::FSM::InputHandler
	{
		std::vector<Gear::Ref<Blob>> blobs;
		int blobCount = 0;
		float blobAddDelay = 0.1f;
		float pastTime = 0.3f;
		float shootPower = 0.0f;
		const float powerRate = 3.0f;

		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::Status>	status;
		Gear::Ref<Gear::Animator> animator;
		Gear::Ref<Gear::FSM> FSM;

		ItemInfo::Number weapon;
		int TimerCheckerID;
		Gear::Ref<Gear::Status> timerStatus;
		WormInfo::DirectionType dir;

		inline float GetAngle(int entityID)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			float angle;

			float nativeFireAngle = std::any_cast<float>(status->GetStat(WormInfo::FireAngle));
			auto WormPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
			{
				angle = 270.0f - nativeFireAngle / 31.0f * 180.0f;
			}
			else
			{
				angle = (nativeFireAngle - 15.5f) / 15.5f * 90.0f;
			}
			return angle;
		}

		inline void OnOut(int entityID) override
		{
			blobs.clear();
			blobCount = 0;
			blobAddDelay = 0.1f;
			pastTime = 0.3f;
			shootPower = 0.0f;
		}

		inline void AfterShoot(int entityID)
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);

			weapon = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem));

			if (weapon == ItemInfo::Number::Bazooka)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatBazukaWithdraw);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatBazukaWithdraw);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpBazukaWithdraw);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpBazukaWithdraw);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownBazukaWithdraw);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownBazukaWithdraw);
					break;
				}
			}
			if (weapon == ItemInfo::Number::Grenade)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatGrenadeWithdraw);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatGrenadeWithdraw);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpGrenadeWithdraw);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpGrenadeWithdraw);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownGrenadeWithdraw);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownGrenadeWithdraw);
					break;
				}
			}
			if (weapon == ItemInfo::Number::Teleport)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatTeleportAfterUse);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatTeleportAfterUse);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpTeleportAfterUse);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpTeleportAfterUse);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownTeleportAfterUse);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownTeleportAfterUse);
					break;
				}
			}
			if (weapon == ItemInfo::Number::Shotgun)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatShotgunWithraw);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatShotgunWithraw);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpShotgunWithraw);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpShotgunWithraw);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownShotgunWithraw);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownShotgunWithraw);
					break;
				}
				status->SetStat(WormInfo::ShotgunUseCnt, 0);
			}
			if (weapon == ItemInfo::Number::Banana)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatBananaWithdraw);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatBananaWithdraw);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpBananaWithdraw);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpBananaWithdraw);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownBananaWithdraw);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownBananaWithdraw);
					break;
				}
			}
		}

		inline void Awake(int entityID) override
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);

			TimerCheckerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
			timerStatus = Gear::EntitySystem::GetStatus(TimerCheckerID);
			OnAwake = false;
		}


		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			float tick = timer->GetTick();
			float angle = GetAngle(entityID);


			if (cmd.KeyType == WormCommand::UseItem)
			{
				glm::vec3 position = transform->GetPosition();
				pastTime += tick;
				if (pastTime > blobAddDelay)
				{
					pastTime = 0.0f;
					auto blob = EffectPool::GetBlob();
					blob->reset(position, blobCount++);
					blobs.push_back(blob);
				}

				for (auto blob = blobs.begin(); blob != blobs.end(); )
				{
					(*blob)->SetAngle(angle);
					(*blob)->UpDate(tick);
					if (!(*blob)->m_OnUsing)
					{
						blob = blobs.erase(blob);
					}
					else
					{
						++blob;
					}
				}
				for (auto& blob : blobs)
				{
					blob->Render();
				}
				shootPower += 0.1f;
				if (shootPower >= 10.0f)
				{
					shootPower = 10.0f;
					glm::vec3 position = transform->GetPosition();
					auto curItemNumber = std::any_cast<ItemInfo::Number>(Gear::EntitySystem::GetStatus(entityID)->GetStat(WormInfo::SelectedItem));
					auto item = ITEM_POOL->GetItem(curItemNumber);
					auto explosionTime = std::any_cast<float>(status->GetStat(WormInfo::Stat::ItemExplosionTime));

					item->init(position, angle, shootPower * powerRate, entityID, explosionTime);

					FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
					status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
					Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, item->GetID())));
					Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TeamInfoBlinkOff)));
					timerStatus->PushNeedHandleData(1, Gear::Status::StatHandleData(0));

					if (curItemNumber == ItemInfo::Number::Bazooka)
					{
						PLAY_SOUND_NAME("ROCKETRELEASE", WormsSound::wormAct);
					}
					else if (curItemNumber == ItemInfo::Number::Banana || curItemNumber == ItemInfo::Number::Grenade)
					{
						PLAY_SOUND_NAME("THROWRELEASE", WormsSound::wormAct);
					}

					AfterShoot(entityID);
					auto teamName = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));
					int ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
					Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(weapon, teamName)), ItemSelectorID);

					OnOut(entityID);
					return WormState::OnItemWithdraw;
				}
				return WormState::OnUseItem;
			}

			glm::vec3 position = transform->GetPosition();
			auto curItemNumber = std::any_cast<ItemInfo::Number>(Gear::EntitySystem::GetStatus(entityID)->GetStat(WormInfo::SelectedItem));
			auto item = ITEM_POOL->GetItem(curItemNumber);
			auto explosionTime = std::any_cast<float>(status->GetStat(WormInfo::Stat::ItemExplosionTime));
			item->init(position, angle, shootPower * powerRate, entityID, explosionTime);
			FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
			status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
			timerStatus->PushNeedHandleData(1, Gear::Status::StatHandleData(0));
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TeamInfoBlinkOff)));
			if (shootPower >= 4.0f)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, item->GetID())));
			}

			STOP_SOUND_CAHNNEL(WormsSound::wormAct);
			if (curItemNumber == ItemInfo::Number::Bazooka)
			{
				PLAY_SOUND_NAME("ROCKETRELEASE", WormsSound::wormAct);
			}
			else if (curItemNumber == ItemInfo::Number::Banana || curItemNumber == ItemInfo::Number::Grenade)
			{
				PLAY_SOUND_NAME("THROWRELEASE", WormsSound::wormAct);
			}

			AfterShoot(entityID);
			auto teamName = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));
			int ItemSelectorID = Gear::EntitySystem::GetEntityIDFromName("ItemSelector");
			Gear::EventSystem::DispatchEventTo(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(weapon, teamName)), ItemSelectorID);

			OnOut(entityID);
			return WormState::OnItemWithdraw;
		}
	};

	class WormOnAfterDamaged : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Animator2D> animator;

		void Awake(int entityID) override
		{
			status = Gear::EntitySystem::GetStatus(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			OnAwake = false;
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			int hp = std::any_cast<int>(status->GetStat(WormInfo::Hp));

			if (hp == 0)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::WormDie, 0, entityID)));
				return WormState::OnNothing;
			}

			bool myTurn = std::any_cast<bool>(status->GetStat(WormInfo::MyTurn));
			if (myTurn)
			{
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(1.0f);
				timer->Start();
				return WormState::OnTurnOver;
			}
			else
			{
				if (std::any_cast<bool>(status->GetStat(WormInfo::Surrendered)))
				{
					auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
					switch (dir)
					{
					case InGame::WormInfo::LeftFlat:
						animator->PlayAnimation(WormState::OnLeftFlatSurrenderOn);
						break;
					case InGame::WormInfo::RightFlat:
						animator->PlayAnimation(WormState::OnRightFlatSurrenderOn);
						break;
					case InGame::WormInfo::LeftUp:
						animator->PlayAnimation(WormState::OnLeftUpSurrenderOn);
						break;
					case InGame::WormInfo::RightUp:
						animator->PlayAnimation(WormState::OnRightUpSurrenderOn);
						break;
					case InGame::WormInfo::LeftDown:
						animator->PlayAnimation(WormState::OnLeftDownSurrenderOn);
						break;
					case InGame::WormInfo::RightDown:
						animator->PlayAnimation(WormState::OnRightDownSurrenderOn);
						break;
					}
				}
				return WormState::OnNotMyTurn;
			}
		}
	};

	class WormOnAirItemUseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnTurnOverHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (!Gear::EntitySystem::GetTimer(entityID)->isExpired())
			{
				return WormState::OnTurnOver;
			}
			GR_TRACE("Worm On tunrover");
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			physics->SetExternalVector({ 0.0f, 0.0f });
			physics->SetPixelCollisionHandler("Move");
			status->SetStat(WormInfo::MyTurn, false);
			if (GameMode::Bit::ModeBit == GameMode::NetWork)
			{
				Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::NetController });
			}
			else
			{
				Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
			}

			WormInfo::DirectionType dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Stat::Direction));
			switch (dir)
			{
			case WormInfo::DirectionType::LeftFlat:
				animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
				break;
			case WormInfo::DirectionType::LeftUp:
				animator->SetCurrentAnimation(WormState::OnLeftUpBreath);
				break;
			case WormInfo::DirectionType::LeftDown:
				animator->SetCurrentAnimation(WormState::OnLeftDownBreath);
				break;
			case WormInfo::DirectionType::RightFlat:
				animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
				break;
			case WormInfo::DirectionType::RightUp:
				animator->SetCurrentAnimation(WormState::OnRightUpBreath);
				break;
			case WormInfo::DirectionType::RightDown:
				animator->SetCurrentAnimation(WormState::OnRightDownBreath);
				break;
			}
			animator->ResumeAnimation();

			int worldID = Gear::EntitySystem::GetEntityIDFromName("World");
			auto worldFSM = Gear::EntitySystem::GetFSM(worldID);
			worldFSM->SetCurrentState(WorldState::OnWaiting);

			return WormState::OnNotMyTurn;
		}
	};

	class WormOnUnderWaterHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Status> status;

		bool sendDyeEvent = false;
		bool damageDisplay = false;
		int worldID;

		void Awake(int entityID) override
		{
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			worldID = Gear::EntitySystem::GetEntityIDFromName("World");
			OnAwake = false;
		}

		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

	class WormOnReadyHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			return WormState::OnReady;
		}
	};

	class WormOnWaitingHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Status> status;
		bool startSound = false;

		inline void Awake(int entityID)
		{
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			OnAwake = false;
		}

		inline void OnOut(int entityID) override
		{
			status->SetStat(WormInfo::MyTurn, true);
			status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(0.5f, 0.0f)));
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::RunningStart, 0, entityID)));
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}
			if (!startSound)
			{
				startSound = true;
				PLAY_SOUND_NAME("YESSIR", WormsSound::wormSpeech);
			}


			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::Stat::MoveSpeed));

			if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT) || Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT))
			{
				startSound = false;
				OnOut(entityID);
				return WormState::OnBreath;
			}
			if (cmd.KeyType != NONE_COMMAND)
			{
				OnOut(entityID);
			}
			if (cmd.KeyType == WormCommand::Left)
			{
				startSound = false;
				physics->SetPixelCollisionHandler("Move");
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				startSound = false;
				physics->SetPixelCollisionHandler("Move");
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				startSound = false;
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::BackJump)
			{
				startSound = false;
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyBackJump;
			}


			return WormState::OnWaiting;
		}
	};

	class WormOnNotMyTurnHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnNotMyTurn;
		}
	};

};
