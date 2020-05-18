#include "wmpch.h"
#include "GrenadeFSMHandlers.h"

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	Gear::EnumType GrenadeOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}
}