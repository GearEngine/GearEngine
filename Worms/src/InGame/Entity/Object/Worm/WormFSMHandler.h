#include "WormEventHandle.h"

namespace InGame {

	class WormOnMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ -1.0f, 0.0f });
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 1.0f, 0.0f });
				return WormState::OnMove;
			}
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnIdle);
			return WormState::OnIdle;
		}
	};

	class WormOnDyeHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

	class WormOnAirHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

	class WormOnIdleHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnMove);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnMove);
				return WormState::OnMove;
			}
			//temporary
			if (cmd.KeyType == WormCommand::ChangeWorm)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
				return WormState::OnTurnOver;
			}

			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			return WormState::OnIdle;
		}
	};

	class WormOnReadyItemUseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

	class WormOnUseItemHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

	class WormOnReadyJumpHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

	class WormOnAirItemUseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

	class WormOnTurnOverHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ 0.0f, 0.0f });
			Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnIdle);
			return WormState::OnTurnOver;
		}
	};

	class WormOnDamagedHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
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
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnMove);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnMove);
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
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
			return WormState::OnNotMyTurn;
		}
	};

}
