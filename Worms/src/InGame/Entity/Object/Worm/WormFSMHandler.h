#include "WormEventHandle.h"

namespace InGame {

	class WormOnMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);

			if (cmd.KeyType == NONE_COMMAND)
			{
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				return WormState::OnBreath;
			}

			WormInfo::DirectionType dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Stat::Direction));
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

			float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::MoveSpeed));
			if (cmd.KeyType== WormCommand::Left)
			{
				physics->SetExternalVector(glm::vec2(-moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				physics->SetExternalVector(glm::vec2(moveSpeed, 0.0f));
				status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
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

			return WormState::OnBreath;
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

	class WormOnReadyJumpHandler : public Gear::FSM::InputHandler
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
			Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			physics->SetExternalVector({ 0.0f, 0.0f });
			physics->SetPixelCollisionHandler("OnAir");
			status->PopNeedHandleData(WormStatusHandleType::WaitingDisplay);

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
				return WormState::OnReadyJump;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
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
