#include "wmpch.h"
#include "WormFSMHandler.h"
#include "InGame/Entity/World/World.h"

Gear::EnumType InGame::WormOnUnderWaterHandler::Handle(int entityID, const Gear::Command & cmd)
{
	if (OnAwake)
	{
		Gear::EventSystem::DispatchEvent(EventType::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TeamInfoBlinkOff)));
		Awake(entityID);
	}

	if (transform->GetPosition().y < -17.0f && !damageDisplay)
	{
		int hp = std::any_cast<int>(status->GetStat(WormInfo::Hp));
		status->SetStat(WormInfo::Damage, 0);
		status->SetStat(WormInfo::SelfDamage, hp);

		status->PushNeedHandleData(WormStatusHandleType::DisplayDamage, Gear::Status::StatHandleData(1));
		damageDisplay = true;
	}

	if (transform->GetPosition().y < -19.f)
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
			if (curState == WormState::OnNothing || curState == WormState::OnUnderWater)
			{
				++damagedWormCount;
			}
		}

		if (!damagedWormCount)
		{
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
		}

		return WormState::OnNothing;
	}
	return WormState::OnUnderWater;
}
