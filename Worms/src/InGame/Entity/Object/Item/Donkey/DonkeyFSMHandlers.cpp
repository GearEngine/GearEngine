#include "wmpch.h"
#include "DonkeyFSMHandlers.h"

namespace InGame {

	Gear::EnumType DonkeyOnGoingHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		return Item::State::OnGoing;
	}

}