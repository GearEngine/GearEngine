#include "wmpch.h"
#include "WormFSMHandler.h"
#include "InGame/Entity/World/World.h"

Gear::EnumType InGame::WormOnUnderWaterHandler::Handle(int entityID, const Gear::Command & cmd)
{
	if (OnAwake)
	{
		Awake(entityID);
	}

	if (transform->GetPosition().y < -18.f)
	{
		Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::WormDie, 0, entityID)));
		Gear::EntitySystem::RegisterInActivateEntity(entityID);

		int damagedWormCount = 0;

		for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
		{
			if (WorldWormData::s_LivingWorms[i] == entityID)
			{
				continue;
			}
			auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
			if (curState == WormState::OnNothing)
			{
				++damagedWormCount;
			}
		}

		if (!damagedWormCount)
		{
			GR_TRACE("Worm FSM under water Dispatch new start!");
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
		}
		

		return WormState::OnNothing;
	}
	return WormState::OnUnderWater;
}
