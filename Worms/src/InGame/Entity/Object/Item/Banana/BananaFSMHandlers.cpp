#include "wmpch.h"
#include "BananaFSMHandlers.h"

#include "../Item.h"
#include "BabyBanana.h"

namespace InGame {

	Gear::EnumType BananaOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		auto BabyBananas = std::any_cast<std::vector<Gear::Ref<BabyBanana>>>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::Baby));

		if (InFirst)
		{
			auto Transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto Position = Transform->GetPosition();
			for (auto& BabyBanana : BabyBananas)
			{
				BabyBanana->init(Position, Gear::Util::GetRndFloatFromTo(70.0f, 110.0f), Gear::Util::GetRndFloatFromTo(10.f, 15.0f), entityID);
			}
		}
		
		for (auto& BabyBanana : BabyBananas)
		{
			int BabyBananaID = BabyBanana->GetID();
			if (!Gear::EntitySystem::IsEntityActivated(BabyBananaID))
			{
				return Item::State::OnExplosion;
			}
		}
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		InFirst = true;
		return Item::State::OnExplosion;
	}

}