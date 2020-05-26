#include "wmpch.h"
#include "BabyHosFSMHandlers.h"

#include "../Item.h"
#include "BabyHos.h"

namespace InGame {

	Gear::EnumType BabyHosOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}

}