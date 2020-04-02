#include "WormEventHandle.h"

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

			if (cmd.KeyType == NONE_COMMAND)
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
			return WormState::OnAir;
		}
	};

	class WormOnBreathHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);

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

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::MoveSpeed));
			if (cmd.KeyType == WormCommand::Left)
			{
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Jump)
			{
				timer->SetTimer(0.2f);
				timer->Start();
				return WormState::OnReadyJump;
			}
			if (cmd.KeyType == WormCommand::BackJump)
			{
				timer->SetTimer(0.2f);
				timer->Start();
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
			static glm::vec2 externalVector(0.0f, 0.0f);

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
				externalVector = { 0.0f, 0.0f };
				onFirst = true;
				backJump = false;
				return WormState::OnJump;
			}

			if (onFirst)
			{
				onFirst = false;
				externalVector.x = 3.4f;
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
				return WormState::OnBreath;
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
				externalX *= 0.95f;
			}

			if (std::abs(externalX) < 0.1f)
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
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			if (animator->loopCompleted())
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("Move");
				return WormState::OnBreath;
			}
			return WormState::OnStuck;
		}
	};

	class WormOnReadyFallenHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);

			auto& externalVector = physics->GetExternalVector();
			float verticalRatio = externalVector.x / externalVector.y * 100.0f;
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			externalVector.y = 0.0f;
			physics->ResetGravityAccelation();

			if (std::abs(verticalRatio) < 4.0f)
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
				return WormState::OnStuck;
			}
			else
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatSliding);
					break;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatSliding);
					break;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpSliding);
					break;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpSliding);
					break;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownSliding);
					break;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownSliding);
					break;
				}
				physics->SetPixelCollisionHandler("Sliding");
				return WormState::OnSliding;
			}

			return WormState::OnFallen;
		}
	};

	class WormOnReadyItemUseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
		}
	};

	class WormOnUseItemHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnLeftFlatBreath;
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
			status->PopNeedHandleData(WormStatusHandleType::WaitingDisplay);
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

			firstIn = true;
			return WormState::OnNotMyTurn;
		}
	};

	class WormOnDamagedHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnDamaged;
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
				status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(0.5f));
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
