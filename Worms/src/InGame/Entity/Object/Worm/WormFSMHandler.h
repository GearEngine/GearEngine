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
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnDye;
		}
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

	class WormOnBreathHandler : public Gear::FSM::InputHandler
	{
		inline void SetReadyItemUseAnimation(int entityID, WormInfo::DirectionType dir)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			Item::Name curSettedItem = std::any_cast<Item::Name>(status->GetStat(WormInfo::SelectedItem));

			if (curSettedItem == Item::Bazooka)
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

		}

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

			if (timer->isExpired())
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
					if(dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
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

		inline void OnOut(int entityID) override
		{
			inFirst = true;
			SettedAngle = false;
			afterReadyAni = false;
			Gear::EntitySystem::GetStatus(entityID)->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
		}

		inline void SetWeaponOnAnimator(int entityID)
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto item = std::any_cast<Item::Name>(status->GetStat(WormInfo::SelectedItem));
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (item == Item::Bazooka)
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
		}

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
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
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(0.2f);
				timer->Start();
				OnOut(entityID);
				return WormState::OnReadyBackJump;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				auto timer = Gear::EntitySystem::GetTimer(entityID);
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

			}
			if (cmd.KeyType == WormCommand::SetTimer2)
			{

			}
			if (cmd.KeyType == WormCommand::SetTimer3)
			{

			}
			if (cmd.KeyType == WormCommand::SetTimer4)
			{

			}
			if (cmd.KeyType == WormCommand::SetTimer5)
			{

			}

			return WormState::OnReadyItemUse;
		}
	};

	class WormOnItemWithdraw : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);

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
				return WormState::OnNothing;
			}
			return WormState::OnItemWithdraw;
		}
	};

	class WormOnAttackedHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnAttacked;
		}
	};

	class WormOnUseItemHandler : public Gear::FSM::InputHandler
	{
		std::vector<Gear::Ref<Blob>> blobs;
		int blobCount = 0;
		float blobAddDelay = 0.1f;
		float pastTime = 0.3f;
		float shoutPower = 0.0f;
		const float powerRate = 3.0f;

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
			shoutPower = 0.0f;
		}

		inline void AfterShoot(int entityID)
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			
			auto weapon = std::any_cast<Item::Name>(status->GetStat(WormInfo::SelectedItem));
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (weapon == Item::Bazooka)
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
		}


		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto tick = Gear::EntitySystem::GetTimer(entityID)->GetTick();
			float angle = GetAngle(entityID);
			auto position = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);

			if (cmd.KeyType == WormCommand::UseItem)
			{
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
				shoutPower += 0.1f;
				if (shoutPower >= 10.0f)
				{
					shoutPower = 10.0f;
					auto item = ITEM_POOL->GetItem(std::any_cast<Item::Name>(Gear::EntitySystem::GetStatus(entityID)->GetStat(WormInfo::SelectedItem)));
					item->init(position, angle, shoutPower * powerRate, entityID);
					status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
					AfterShoot(entityID);

					OnOut(entityID);
					return WormState::OnItemWithdraw;
				}
				return WormState::OnUseItem;
			}

			auto item = ITEM_POOL->GetItem(std::any_cast<Item::Name>(Gear::EntitySystem::GetStatus(entityID)->GetStat(WormInfo::SelectedItem)));
			item->init(position, angle, shoutPower * powerRate, entityID);
			status->SetNeedHandleData(WormStatusHandleType::DisplayAim, true);
			AfterShoot(entityID);

			OnOut(entityID);
			return WormState::OnItemWithdraw;
		}
	};

	class WormOnAfterDamaged : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			GR_TRACE("FSM on After Damaged Worm : {0}", entityID);

			auto status = Gear::EntitySystem::GetStatus(entityID);
			
			int hp = std::any_cast<int>(status->GetStat(WormInfo::Hp));
						
			if (hp == 0)
			{
				return WormState::OnDye;
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
			static bool firstIn = true;
			if (firstIn)
			{
				firstIn = false;
				Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
			}

			if (!Gear::EntitySystem::GetTimer(entityID)->isExpired())
			{
				return WormState::OnTurnOver;
			}

			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			physics->SetExternalVector({ 0.0f, 0.0f });
			physics->SetPixelCollisionHandler("Move");
			status->SetStat(WormInfo::MyTurn, false);

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
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));

			firstIn = true;
			return WormState::OnNotMyTurn;
		}
	};

	class WormOnUnderWaterHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			if (transform->GetPosition().y < -18.f)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
				Gear::EntitySystem::RegisterInActivateEntity(entityID);
				return WormState::OnTurnOver;
			}
			return WormState::OnUnderWater;
		}
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
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::Stat::MoveSpeed));

			if (cmd.KeyType != NONE_COMMAND)
			{
				status->SetStat(WormInfo::MyTurn, true);
				GR_TRACE("Push DisplayPosChage +0.5f at Worm FSM OnWaiting Handler");
				status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(0.5f, 0.0f)));
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::RunningStart, 0, entityID)));
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
