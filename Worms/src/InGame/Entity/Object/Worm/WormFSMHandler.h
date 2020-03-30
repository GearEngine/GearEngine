#include "WormEventHandle.h"

namespace InGame {

	class WormOnLeftFlatMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ -1.0f, 0.0f });
				return WormState::OnLeftFlatMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 1.0f, 0.0f });
				return WormState::OnRightFlatMove;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnLeftFlatBreath);
			return WormState::OnLeftFlatMove;
		}
	};

	class WormOnRightFlatMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ -1.0f, 0.0f });
				return WormState::OnLeftFlatMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 1.0f, 0.0f });
				return WormState::OnRightFlatMove;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnRightFlatBreath);
			return WormState::OnRightFlatBreath;
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

	class WormOnLeftFlatBreathHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnLeftFlatMove);
				return WormState::OnLeftFlatMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnRightFlatMove);
				return WormState::OnRightFlatMove;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			return WormState::OnLeftFlatBreath;
		}
	};
	class WormOnRightFlatBreathHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnLeftFlatMove);
				return WormState::OnLeftFlatMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnRightFlatMove);
				return WormState::OnRightFlatMove;
			}

			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			return WormState::OnRightFlatBreath;
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
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnLeftFlatBreath);
			Gear::EntitySystem::GetStatus(entityID)->PopNeedHandleData(WormStatusHandleType::WaitingDisplay);

			return WormState::OnTurnOver;
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
			if (cmd.Keycode != 0xffffffb)
			{
				auto& status = Gear::EntitySystem::GetStatus(entityID);
				status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(0.5f));
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::RunningStart, 0, entityID)));
			}
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnLeftFlatMove);
				return WormState::OnLeftFlatMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnRightFlatMove);
				return WormState::OnRightFlatMove;
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
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
			return WormState::OnNotMyTurn;
		}
	};

}
