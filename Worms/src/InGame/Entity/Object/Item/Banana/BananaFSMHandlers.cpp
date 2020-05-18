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
			PLAY_SOUND_NAME("Explosion" + std::to_string(Gear::Util::GetRndInt(3) + 1), WormsSound::Weapon);
			for (auto& BabyBanana : BabyBananas)
			{
				BabyBanana->init(Position, Gear::Util::GetRndFloatFromTo(80.0f, 100.0f), Gear::Util::GetRndFloatFromTo(13.f, 18.0f), entityID);
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