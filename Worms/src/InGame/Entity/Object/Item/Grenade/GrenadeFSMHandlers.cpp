#include "wmpch.h"
#include "GrenadeFSMHandlers.h"

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	Gear::EnumType GrenadeOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		PLAY_SOUND_NAME("Explosion" + std::to_string(Gear::Util::GetRndInt(3) + 1), WormsSound::Weapon);
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}

}