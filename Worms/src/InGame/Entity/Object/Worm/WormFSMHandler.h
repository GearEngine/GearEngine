#include "WormEnum.h"

namespace InGame {

	class WormOnMoveHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (cmd.KeyType == WormCommand::Left)
			{
				GR_TRACE("{0}entity(Worm) On Move state have Left Command! ", entityID);
				Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVector({ -1.0f, 0.0f });
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				GR_TRACE("{0}entity(Worm) On Move state have Right Command! ", entityID);
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
				GR_TRACE("{0}entity(Worm) On idle state have Left Command! ", entityID);
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnMove);
				return WormState::OnMove;
			}
			if (cmd.KeyType == WormCommand::Right)
			{
				GR_TRACE("{0}entity(Worm) On idle state have Right Command! ", entityID);
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnMove);
				return WormState::OnMove;
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
			return WormState::OnIdle;
		}
	};

	class WormOnDamagedHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WormState::OnIdle;
		}
	};

}
