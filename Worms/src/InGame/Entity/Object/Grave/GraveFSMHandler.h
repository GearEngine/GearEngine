#pragma once
#include "GraveEventHandler.h"

namespace InGame {

	class GraveOnNothingHandler : public Gear::FSM::InputHandler
	{
		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return GraveInfo::State::OnNothing;
		}
	};

	class GraveOnUnderWaterHandler : public Gear::FSM::InputHandler
	{
		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return GraveInfo::State::OnUnderWater;
		}
	};

}