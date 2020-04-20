#include "WormEventHandle.h"
#include "InGame/Entity/Object/Effects/Effects.h"
#include "InGame/Entity/Object/Item/Item.h"

namespace InGame {

	class WormOnMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);

			if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
			{
				return WormState::OnBreath;
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
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::BackJump)
			{
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
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatBazukaReady);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatBazukaReady);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpBazukaReady);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpBazukaReady);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownBazukaReady);
				break;
			case InGame::WormInfo::RightDown:
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
					backFlip = true;
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
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			GR_TRACE("FSM on Stuck Worm : {0}", entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			if (animator->loopCompleted())
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("Move");
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
		const float FixedFireAngle = 15.0f;

		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Timer> timer;

		int timerCheckerID;
		Gear::Ref<Gear::Status> timerStatus;

		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);

			timerCheckerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
			timerStatus = Gear::EntitySystem::GetStatus(timerCheckerID);
			OnAwake = false;
		}

		inline void OnOut(int entityID) override
		{
			inFirst = true;
			SettedAngle = false;
			afterReadyAni = false;
			status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
		}

		inline void SetWeaponOnAnimator(int entityID)
		{
			
			auto item = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem));
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (item == ItemInfo::Number::Bazooka)
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
			}
			if (item == ItemInfo::Number::Grenade)
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
			}
			if (item == ItemInfo::Number::Banana)
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
				status->SetNeedHandleData(WormStatusHandleType::DisplayAim, false);
				firstFireAngle = FixedFireAngle;
			}

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::Stat::MoveSpeed));

			if (animator->loopCompleted())
			{
				SetWeaponOnAnimator(entityID);
				afterReadyAni = true;
			}

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
				timerStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
				return WormState::OnUseItem;
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

			if (cmd.KeyType == WormCommand::SetTimer1)
			{
				status->SetStat(WormInfo::Stat::ItemExplosionTime, 1.0f);
			}
			if (cmd.KeyType == WormCommand::SetTimer2)
			{
				status->SetStat(WormInfo::Stat::ItemExplosionTime, 2.0f);
			}
			if (cmd.KeyType == WormCommand::SetTimer3)
			{
				status->SetStat(WormInfo::Stat::ItemExplosionTime, 3.0f);
			}
			if (cmd.KeyType == WormCommand::SetTimer4)
			{
				status->SetStat(WormInfo::Stat::ItemExplosionTime, 4.0f);
			}
			if (cmd.KeyType == WormCommand::SetTimer5)
			{
				status->SetStat(WormInfo::Stat::ItemExplosionTime, 5.0f);
			}

			return WormState::OnReadyItemUse;
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
			status->SetStat(WormInfo::Stat::UsedItem, true);
			status->PushNeedHandleData(WormStatusHandleType::AfterUseItem, Gear::Status::StatHandleData(2.0f));
			return WormState::OnBreath;
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

		inline float GetAngle(int entityID)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			float angle;

			float nativeFireAngle = std::any_cast<float>(status->GetStat(WormInfo::FireAngle));
			auto WormPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

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
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

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
					auto item = ITEM_POOL->GetItem(std::any_cast<ItemInfo::Number>(Gear::EntitySystem::GetStatus(entityID)->GetStat(WormInfo::SelectedItem)));
					auto explosionTime = std::any_cast<float>(status->GetStat(WormInfo::Stat::ItemExplosionTime));
					item->init(position, angle, shootPower * powerRate, entityID, explosionTime);

					FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
					status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
					Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, item->GetID())));
					Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TeamInfoBlinkOff)));
					timerStatus->PushNeedHandleData(1, Gear::Status::StatHandleData(0));

					AfterShoot(entityID);
					auto teamName = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));
					Gear::EventSystem::DispatchEvent(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(weapon, teamName)));

					OnOut(entityID);
					return WormState::OnItemWithdraw;
				}
				return WormState::OnUseItem;
			}

			glm::vec3 position = transform->GetPosition();
			auto item = ITEM_POOL->GetItem(std::any_cast<ItemInfo::Number>(Gear::EntitySystem::GetStatus(entityID)->GetStat(WormInfo::SelectedItem)));
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

			AfterShoot(entityID);
			auto teamName = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));
			Gear::EventSystem::DispatchEvent(EventChannel::Worm, Gear::EntityEvent(EventType::Worm, UseItemData(weapon, teamName)));

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
			Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });

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
			worldFSM->SetCurrentState(8u);

			return WormState::OnNotMyTurn;
		}
	};

	class WormOnUnderWaterHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Status> status;

		bool damageDisplay = false;

		void Awake(int entityID) override
		{
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
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
			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::Stat::MoveSpeed));
			
			if (cmd.KeyType != NONE_COMMAND)
			{
				OnOut(entityID);
			}
			if (cmd.KeyType == WormCommand::Left)
			{
				physics->SetPixelCollisionHandler("Move");
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetPixelCollisionHandler("Move");
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::BackJump)
			{
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
}
