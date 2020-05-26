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

		if (!IS_PLAYING_SOUND(WormsSound::wormAct))
		{
			PLAY_SOUND_NAME("UnderWaterLoop", WormsSound::wormAct);
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
			STOP_SOUND_CAHNNEL(WormsSound::wormAct);
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
				bool aleadySend = false;
				auto curState = Gear::EntitySystem::GetFSM(worldID)->GetCurrentState();
				auto usedItem = std::any_cast<bool>(status->GetStat(WormInfo::UsedItem));
				auto myTurn = std::any_cast<bool>(status->GetStat(WormInfo::MyTurn));
				if (!WorldWormData::s_WaitingDyeQue.size())
				{
					if (!usedItem && myTurn)
					{
						Gear::EventSystem::DispatchEventOnce(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
						GR_TRACE("Worm On Underwater : Dispatch new Start");
					}
					if (curState != WorldState::OnPrepareRun)
					{
						Gear::EventSystem::DispatchEventOnce(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
						GR_TRACE("Worm On Underwater : Dispatch new Start");
					}
				}
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

		if (!dyeSound)
		{
			dyeSound = true;
			if (Gear::Util::GetRndInt(2))
			{
				PLAY_SOUND_NAME("BYE1", WormsSound::wormSpeech);
			}
			else
			{
				PLAY_SOUND_NAME("BYE2", WormsSound::wormSpeech);
			}
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

			GRAVE_POOL->ActivateGrave(std::any_cast<GraveInfo::Type>(status->GetStat(WormInfo::Grave)), wormPosition);

			Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(position, Explosion::Size::Size25, 25.0f)));
			
			dyeSound = false;
			Gear::EntitySystem::RegisterInActivateEntity(entityID);
		}

		return WormState::OnDye;
	}

	glm::vec2 WormOnUseShotGun::hitPosition;
	void WormOnUseShotGun::calcShotPoint(Gear::Ref<Gear::Texture2D> mask, const glm::mat4& textureTrasform, const glm::vec3 & position, float angle)
	{
		int width = mask->GetWidth();
		int height = mask->GetHeight();
		
		auto texturePos = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(glm::vec2(position.x, position.y), textureTrasform);
		glm::vec3 targetColor = glm::vec3(255.0f, 255.0f, 255.0f);
		glm::vec3 pixel;
		float bulletPositionX = texturePos.x * width;
		float bulletPositionY = texturePos.y * height;

		float normalX = cosf(glm::radians(angle));
		float normalY = sinf(glm::radians(angle));

		while (1)
		{
			bulletPositionX += normalX;
			bulletPositionY += normalY;

			pixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { (int)bulletPositionX , (int)bulletPositionY });
			if (pixel == targetColor)
			{
				float localX = (bulletPositionX) / width - 0.5f;
				float localY = (bulletPositionY) / height - 0.5f;

				hitPosition.x = textureTrasform[0][0] * localX + textureTrasform[3][0];
				hitPosition.y = textureTrasform[1][1] * localY + textureTrasform[3][1];

				break;
			}
			if (bulletPositionX < 0 || bulletPositionX > width || bulletPositionY < 0 || bulletPositionY >height)
			{
				break;
			}
		}
	}
	void WormOnUseShotGun::Shot(int entityID)
	{
		ExplosionData data(hitPosition, Explosion::Size25, 25.0f, ItemInfo::Shotgun, entityID);

		ITEM_POOL->MakeExplosion(data, hitPosition, Explosion::Text::Null, "None");
	}
}

