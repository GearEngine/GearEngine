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
				auto explosionRadius = std::any_cast<float>(status->GetStat(Item::Info::ExplosionRadius));
				auto power = std::any_cast<float>(status->GetStat(Item::Info::Power));

				//TODO : edit hardcord (Explosion::Size50, Explosion::Text::Foom)
				Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, ExplosionData(checkPosition, Explosion::Size50, Explosion::Text::Foom, power)));
				auto explosion = EffectPool::GetExplosion(Explosion::Size50, Explosion::Text::Foom);
				explosion->init(checkPosition);
				ObjectLayer::s_Explosion.push_back(explosion);

				FSM->SetCurrentState(Item::State::OnExplosion);
			}

		}
	};

}