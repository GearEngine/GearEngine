#include "wmpch.h"
#include "HosFSMHandlers.h"

#include "../Item.h"

namespace InGame {

	Gear::EnumType HosOnGoingHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}
		float& angle = Transform->GetRotation();
		angle += 30.0f * timer->GetTick();

		return Item::State::OnGoing;
	}

	Gear::EnumType HosOnExplosionHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}

		for (int i = 0; i < 6; ++i)
		{
			ITEM_POOL->GetItem(ItemInfo::BabyHos)->init(Transform->GetPosition(), Gear::Util::GetRndFloatFromTo(30.0f, 150.0f), Gear::Util::GetRndFloatFromTo(10.f, 30.0f), entityID);
		}

		return Item::State::OnGoing;
	}

}