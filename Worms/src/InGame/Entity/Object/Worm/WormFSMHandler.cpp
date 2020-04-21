#include "wmpch.h"
#include "WormFSMHandler.h"

#include "InGame/Entity/World/World.h"
#include "InGame/Entity/Object/Grave/Grave.h"

namespace InGame {

	Gear::EnumType WormOnUnderWaterHandler::Handle(int entityID, const Gear::Command & cmd)
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

		if (transform->GetPosition().y < -18.5f && !sendDyeEvent)
		{
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::WormDie, 0, entityID)));
			sendDyeEvent = true;
		}

		if (transform->GetPosition().y < -19.f)
		{
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

	Gear::EnumType WormOnDyeHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}
		if (animator->loopCompleted())
		{
			auto wormPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			glm::vec2 position(wormPosition.x, wormPosition.y - 0.3f);

			auto explosion = EffectPool::GetExplosion(Explosion::Size25, Explosion::Text::Null);
			auto smoke = EffectPool::GetExplosionSmoke(Explosion::Size25);
			auto flame = EffectPool::GetFlame(Explosion::Size25);

			smoke->init(position);
			explosion->init(position);
			flame->init(position);

			EffectPool::pushExplosion(explosion);
			EffectPool::pushExplosionSmoke(smoke);
			EffectPool::pushFlame(flame);

			GRAVE_POOL->ActivateGrave(GraveInfo::_1, wormPosition);

			Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(position, Explosion::Size::Size25, 25.0f)));

			Gear::EntitySystem::RegisterInActivateEntity(entityID);
		}

		return WormState::OnDye;
	}


}
