#include "wmpch.h"
#include "BabyBananaFSMHandlers.h"

#include "../Item.h"
#include "BabyBanana.h"

namespace InGame {

	Gear::EnumType BabyBananaOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		PLAY_SOUND_NAME("Explosion" + std::to_string(Gear::Util::GetRndInt(2) + 1), WormsSound::Weapon);
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}

}