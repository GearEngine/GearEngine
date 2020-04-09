#pragma once

#include "../ItemEnum.h"

namespace InGame {

	class BazookaOnGoingHandler : public Gear::FSM::InputHandler
	{
	public:
		BazookaOnGoingHandler()
		{
			WindAdjustRatio = 0.7f;
		}
	private:
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
		float WindAdjustRatio;
	};

	class BazookaOnUnderWater : public Gear::FSM::InputHandler
	{
		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{

			return Item::State::OnUnderWater;
		}
	};

	class BazookaOnExplosion : public Gear::FSM::InputHandler
	{
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};
}