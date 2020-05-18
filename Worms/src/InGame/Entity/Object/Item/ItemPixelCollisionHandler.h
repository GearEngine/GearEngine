#pragma once

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	class MissileTypePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		float radius = 0.5f;
		inline virtual void Handle(int entityID) override
		{
			if (m_TargetPos->y < -13.0f)
			{
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnUnderWater);
				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("GeneralWeaponUnderWater");
				int shooter = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
				auto shooterStatus = Gear::EntitySystem::GetStatus(shooter);

				shooterStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(std::make_pair(-0.2f, 0.0f)));

				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			float angle = Gear::Util::GetAngleFromXY(m_ExternalVector->x, m_ExternalVector->y);

			glm::vec2 checkPosition(m_TargetPos->x + radius * glm::cos(glm::radians(angle)), m_TargetPos->y + radius * glm::sin(glm::radians(angle)));

			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);

			float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });

			if (pixel == m_TargetPixelColor)
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				auto power = std::any_cast<float>(status->GetStat(Item::Info::Power));
				auto from = std::any_cast<int>(status->GetStat(Item::Info::From));
				auto number = (unsigned int)std::any_cast<ItemInfo::Number>(status->GetStat(Item::Info::Number));
				auto size = std::any_cast<Explosion::Size>(status->GetStat(Item::Info::ExplosionSize));
				auto text = std::any_cast<Explosion::Text>(status->GetStat(Item::Info::ExplosionText));

				Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(checkPosition, size, power, number, from)));
				auto explosion = EffectPool::GetExplosion(size, text);
				explosion->init(checkPosition);
				ObjectLayer::s_Explosion.push_back(explosion);

				auto smoke = EffectPool::GetExplosionSmoke(size);
				smoke->init(checkPosition);
				ObjectLayer::s_ExplosionSmoke.push_back(smoke);

				auto flame = EffectPool::GetFlame(size);
				flame->init(checkPosition);
				ObjectLayer::s_Flames.push_back(flame);

				FSM->SetCurrentState(Item::State::OnExplosion);

				int shooterID = std::any_cast<int>(status->GetStat(Item::Info::From));
				auto shooterStatus = Gear::EntitySystem::GetStatus(shooterID);

				shooterStatus->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
			}
		}
	};

	class GrenadeTypePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		float radius = 0.1f;
		bool explosionDone = false;
		bool awake = true;
		ItemInfo::Number itemNumber;

		void Awake(int entityID)
		{
			itemNumber = std::any_cast<ItemInfo::Number>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::Number));
		}

		inline virtual void Handle(int entityID) override
		{
			if (awake)
			{
				Awake(entityID);
			}
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			if (timer->isExpired())
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				auto power = std::any_cast<float>(status->GetStat(Item::Info::Power));
				auto from = std::any_cast<int>(status->GetStat(Item::Info::From));
				auto number = (unsigned int)std::any_cast<ItemInfo::Number>(status->GetStat(Item::Info::Number));
				auto size = std::any_cast<Explosion::Size>(status->GetStat(Item::Info::ExplosionSize));
				auto text = std::any_cast<Explosion::Text>(status->GetStat(Item::Info::ExplosionText));
				auto& position = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();

				Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(position, size, power, number, from)));
				auto explosion = EffectPool::GetExplosion(size, text);
				explosion->init(position);
				ObjectLayer::s_Explosion.push_back(explosion);

				auto smoke = EffectPool::GetExplosionSmoke(size);
				smoke->init(position);
				ObjectLayer::s_ExplosionSmoke.push_back(smoke);

				auto flame = EffectPool::GetFlame(size);
				flame->init(position);
				ObjectLayer::s_Flames.push_back(flame);
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnExplosion);

				int shooterID = std::any_cast<int>(status->GetStat(Item::Info::From));

				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("GeneralWeaponUnderWater");

				auto shooterStatus = Gear::EntitySystem::GetStatus(shooterID);
				shooterStatus->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
				return;
			}

			if (m_TargetPos->y < -13.0f)
			{
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnUnderWater);
				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("GeneralWeaponUnderWater");
				int shooter = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
				auto shooterStatus = Gear::EntitySystem::GetStatus(shooter);

				shooterStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(std::make_pair(-0.2f, 0.0f)));

				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

			glm::vec2 checkPosition;
			glm::vec3 pixel;
			float ItemOnTexturePositionX;
			float ItemOnTexturePositionY;
			bool leftCollision = false, rightCollision = false;
			bool downCollision = false, upCollision = false;
			
			//Up 
			checkPosition.x = m_TargetPos->x;
			checkPosition.y = m_TargetPos->y;
			if (m_ExternalVector->y > 0.0f)
			{
				checkPosition.y = m_TargetPos->y + radius;
				auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);
				ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
				ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;
				pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });
				if (pixel == m_TargetPixelColor)
				{
					int fixedYPos = 0;
					for (int i = 1; i <= 40; ++i)
					{
						fixedYPos = ItemOnTexturePositionY - i;
						pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, fixedYPos });
						if (pixel != m_TargetPixelColor)
						{
							upCollision = true;
							float localY = (fixedYPos) / m_TargetTextureHeight - 0.5f;
							m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
							m_ExternalVector->y = -m_ExternalVector->y;
							m_ExternalVector->x *= 0.9f;
							break;
						}
					}
				}
			}
			//Down
			else if (m_ExternalVector->y <= 0.0f)
			{
				checkPosition.y = m_TargetPos->y - radius;
				auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);
				ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
				ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;
				pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });
				if (pixel == m_TargetPixelColor)
				{
					int fixedYPos = 0;
					for (int i = 1; i <= 40; ++i)
					{
						fixedYPos = ItemOnTexturePositionY + i;
						pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, fixedYPos });
						if (pixel != m_TargetPixelColor)
						{
							float localY = (fixedYPos) / m_TargetTextureHeight - 0.5f;
							m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
							if (m_ExternalVector->y < -7.0f)
							{
								m_ExternalVector->y = -m_ExternalVector->y * 0.5f;
								m_ExternalVector->x *= 0.5f;
							}
							else if(m_ExternalVector->y < -5.0f)
							{
								m_ExternalVector->y = -m_ExternalVector->y * 0.6f;
								m_ExternalVector->x *= 0.6f;
							}
							else if (m_ExternalVector->y < -3.0f)
							{
								m_ExternalVector->y = -m_ExternalVector->y * 0.7f;
								m_ExternalVector->x *= 0.7f;
							}
							else
							{
								m_ExternalVector->y = -m_ExternalVector->y * 0.8f;
								m_ExternalVector->x *= 0.8f;
							}
							downCollision = true;
							break;
						}
					}
				}
			}

			if (downCollision || upCollision)
			{
				if (std::abs(m_ExternalVector->x) + std::abs(m_ExternalVector->y) > 1.0f)
				{
					if (itemNumber == ItemInfo::Number::Banana)
					{
						PLAY_SOUND_NAME("BananaImpact", WormsSound::Weapon);
					}
					if (itemNumber == ItemInfo::Number::Grenade)
					{
						PLAY_SOUND_NAME("GRENADEIMPACT", WormsSound::Weapon);
					}
				}
				return;
			}

			checkPosition.x = m_TargetPos->x;
			checkPosition.y = m_TargetPos->y;
			//left 
			if (m_ExternalVector->x < 0.0f)
			{
				checkPosition.x = m_TargetPos->x - radius;
				auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);
				ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
				ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;
				pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });
				if (pixel == m_TargetPixelColor)
				{
					leftCollision = true;
					int fixedXPos = 0;
					for (int i = 1; i <= 10; ++i)
					{
						fixedXPos = ItemOnTexturePositionX + i;
						pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { fixedXPos, ItemOnTexturePositionY });
						if (pixel != m_TargetPixelColor)
						{
							float localX = (fixedXPos) / m_TargetTextureWidth - 0.5f;
							m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
							break;
						}
					}
				}
			}
			//right
			else if(m_ExternalVector->x > 0.0f)
			{
				checkPosition.x = m_TargetPos->x + radius;
				auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);
				ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
				ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;
				pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });
				if (pixel == m_TargetPixelColor)
				{
					rightCollision = true;
					int fixedXPos = 0;
					for (int i = 1; i < 10; ++i)
					{
						fixedXPos = ItemOnTexturePositionX - i;
						pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { fixedXPos, ItemOnTexturePositionY });
						if (pixel != m_TargetPixelColor)
						{
							float localX = (fixedXPos) / m_TargetTextureWidth - 0.5f;
							m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
							break;
						}
					}
				}
			}
			if (leftCollision || rightCollision)
			{
				if (itemNumber == ItemInfo::Number::Banana)
				{
					PLAY_SOUND_NAME("BananaImpact", WormsSound::Weapon);
				}
				if (itemNumber == ItemInfo::Number::Grenade)
				{
					PLAY_SOUND_NAME("GRENADEIMPACT", WormsSound::Weapon);
				}
				m_ExternalVector->x = -m_ExternalVector->x * 0.8f;
			}


			
		}
	};


	class GaneralWeaponOnUnderWater : public Gear::Physics2D::PixelCollisionHander
	{
		inline virtual void Handle(int entityID) override
		{
			m_ExternalVector->y = -1.3f;
		}
	};

	class BabyTypePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		float radius = 0.5f;
		inline virtual void Handle(int entityID) override
		{
			if (m_TargetPos->y < -13.0f)
			{
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnUnderWater);
				Gear::EntitySystem::GetPhysics2D(entityID)->SetPixelCollisionHandler("GeneralWeaponUnderWater");

				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			float angle = Gear::Util::GetAngleFromXY(m_ExternalVector->x, m_ExternalVector->y);

			glm::vec2 checkPosition(m_TargetPos->x + radius * glm::cos(glm::radians(angle)), m_TargetPos->y + radius * glm::sin(glm::radians(angle)));

			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);

			float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });

			if (pixel == m_TargetPixelColor)
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				auto power = std::any_cast<float>(status->GetStat(Item::Info::Power));
				auto from = std::any_cast<int>(status->GetStat(Item::Info::From));
				auto number = (unsigned int)std::any_cast<ItemInfo::Number>(status->GetStat(Item::Info::Number));
				auto size = std::any_cast<Explosion::Size>(status->GetStat(Item::Info::ExplosionSize));
				auto text = std::any_cast<Explosion::Text>(status->GetStat(Item::Info::ExplosionText));

				Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(checkPosition, size, power, number, from)));
				auto explosion = EffectPool::GetExplosion(size, text);
				explosion->init(checkPosition);
				ObjectLayer::s_Explosion.push_back(explosion);

				auto smoke = EffectPool::GetExplosionSmoke(size);
				smoke->init(checkPosition);
				ObjectLayer::s_ExplosionSmoke.push_back(smoke);

				auto flame = EffectPool::GetFlame(size);
				flame->init(checkPosition);
				ObjectLayer::s_Flames.push_back(flame);

				FSM->SetCurrentState(Item::State::OnExplosion);
			}
		}
	};
}