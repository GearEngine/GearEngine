#pragma once

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	class MissileTypePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		float radius = 0.5f;
		inline virtual void Handle(int entityID) override
		{
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
				auto size = std::any_cast<Explosion::Size>(status->GetStat(Item::Info::ExplosionSize));
				auto text = std::any_cast<Explosion::Text>(status->GetStat(Item::Info::ExplosionText));

				Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(checkPosition, size, text)));
				auto explosion = EffectPool::GetExplosion(size, text);
				explosion->init(checkPosition);
				ObjectLayer::s_Explosion.push_back(explosion);

				auto smoke = EffectPool::GetExplosionSmoke(size);
				smoke->init(checkPosition);
				ObjectLayer::s_ExplosionSmoke.push_back(smoke);

				auto flame = EffectPool::GetFlame(size);
				flame->init(checkPosition);
				ObjectLayer::s_ExplosionFlame.push_back(flame);

				FSM->SetCurrentState(Item::State::OnExplosion);
			}
		}
	};

}