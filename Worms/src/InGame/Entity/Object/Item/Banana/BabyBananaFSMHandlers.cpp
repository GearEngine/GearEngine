#include "wmpch.h"
#include "BabyBananaFSMHandlers.h"

#include "../Item.h"
#include "BabyBanana.h"

namespace InGame {

	Gear::EnumType BabyBananaOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}

}