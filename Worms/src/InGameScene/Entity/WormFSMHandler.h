#include "Gear.h"
#include "WormEnum.h"

class OnMoveHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		if (cmd.KeyType == WormCommand::Left)
		{
			GR_TRACE("{0}entity(Worm) On Move state have Left Command! ", entityID);
			return WormState::OnMove;
		}
		if (cmd.KeyType == WormCommand::Right)
		{
			GR_TRACE("{0}entity(Worm) On Move state have Right Command! ", entityID);
			return WormState::OnMove;
		}

		Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnIdle);
		return WormState::OnIdle;
	}
};

class OnDyeHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnAirHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnIdleHandler : public Gear::FSM::InputHandler
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

		return WormState::OnIdle;
	}
};

class OnReadyItemUseHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnUseItemHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnReadyJumpHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnAirItemUseHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnTurnOverHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnDamagedHandler : public Gear::FSM::InputHandler
{
	inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
	{
		return WormState::OnIdle;
	}
};