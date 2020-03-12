#include "Gear.h"
#include "WormEnum.h"

class OnMoveHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnDyeHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnAirHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnIdleHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnReadyItemUseHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnUseItemHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnReadyJumpHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnAirItemUseHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnTurnOverHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};

class OnDamagedHandler : public Gear::FSM::InputHandler
{
	inline virtual int Handle(Gear::Command cmd) override
	{
		return WormState::OnIdle;
	}
};