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

				shooterStatus->PushNeedHandleData(2, Gear::Status::StatHandleData(std::make_pair( -0.2f, 0.0f )));

				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			float angle = Gear::Util::GetAngleFromXY(m_ExternalVector->x, m_ExternalVector->y);

			GR_TRACE("{0} {1}", m_TargetPos->x, m_TargetPos->y);
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
				
				Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(checkPosition, size, power,number,from)));
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
				
				shooterStatus->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair( -0.2f, 1.0f )));
			}
		}
	};

	class GrenadeTypePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		float radius = 0.2f;
		bool explosionDone = false;

		inline virtual void Handle(int entityID) override
		{
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

			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			float angle = Gear::Util::GetAngleFromXY(m_ExternalVector->x, m_ExternalVector->y);

			//ÆøÅºÀÇ ¿ùµåÁÂÇ¥
			glm::vec2 checkPosition(m_TargetPos->x + radius * glm::cos(glm::radians(angle)), m_TargetPos->y + radius * glm::sin(glm::radians(angle)));

			//¸¶½ºÅ©ÀÇ ·ÎÄÃÁÂÇ¥
			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);

			//¸¶½ºÅ©ÀÇ ÀÎµ¦½º
			float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			//¸¶½ºÅ©ÀÇ ÀÎµ¦½º·Î ÇÈ¼¿°¡Á®¿À±â
			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });

			if (pixel == m_TargetPixelColor)
			{
				//adjustPosition
				float repositionRadius = radius;
				while (1)
				{
					repositionRadius -= 0.1f;
					checkPosition.x = m_TargetPos->x + repositionRadius * glm::cos(glm::radians(angle));
					checkPosition.y = m_TargetPos->y + repositionRadius * glm::sin(glm::radians(angle));

					textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(checkPosition, *m_PixelCollisionTargetTextureTranslate);

					ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
					ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

					pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX, ItemOnTexturePositionY });
					if (pixel != m_TargetPixelColor)
					{
						m_TargetPos->x = checkPosition.x;
						m_TargetPos->y = checkPosition.y;

						auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
						auto& externalVector = physics->GetExternalVector();

						externalVector.x = -externalVector.x * 0.8f;
						externalVector.y = -externalVector.y * 0.7f;

						break;
					}
				}


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