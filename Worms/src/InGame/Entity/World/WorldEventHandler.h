#pragma once

#include "WorldState.h"

namespace InGame {

	class WorldEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
		}
	};


}