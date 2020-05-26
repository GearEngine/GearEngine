#include "wmpch.h"
#include "HosFSMHandlers.h"

namespace InGame {

	Gear::EnumType HosOnGoingHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		return Item::State::OnGoing;
	}

}