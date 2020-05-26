#include "wmpch.h"
#include "ItemPixelCollisionHandler.h"

#include "Item.h"

namespace InGame {

	void InGame::DonkeyTypePCHandler::Handle(int entityID)
	{
		if (m_TargetPos->y < -130.0f)
		{
			auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
			ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.7f), Explosion::Size25, 0.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y), Explosion::Text::Null, "None");

			Gear::EntitySystem::RegisterInActivateEntity(entityID);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::SelectedItem, ItemInfo::Bazooka);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::MyTurn, false);
		}

		m_ExternalVector->y -= 0.53f;

		if (m_ExternalVector->y < -30.0f)
		{
			m_ExternalVector->y = -30.0f;
		}
		if (m_ExternalVector->y > 0.0f || m_TargetPos->y < -20.0f)
		{
			return;
		}
		auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

		float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
		float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

		for (int i = -65; i <= 65; i += 5)
		{
			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i, ItemOnTexturePositionY - 60 });
			
			if (pixel == m_TargetPixelColor)
			{
				int yOffset = 60;
				while (1)
				{
					yOffset -= 2;
					pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i , ItemOnTexturePositionY - yOffset });
					if (pixel != m_TargetPixelColor)
					{
						float localY = (ItemOnTexturePositionY - yOffset - 1) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
						break;
					}
				}
				PLAY_SOUND_NAME("HOLYDONKEYIMPACT", WormsSound::Weapon);
				m_ExternalVector->y = -(m_ExternalVector->y * 0.7f);

				auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
				ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.3f), Explosion::Size100, 100.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y), Explosion::Text::Foom, "");
				break;
			}
		}
		
	}

	void HosPCHandler::Handle(int entityID)
	{
		if (m_TargetPos->y < -130.0f)
		{
			auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
			ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.7f), Explosion::Size25, 0.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y), Explosion::Text::Null, "None");

			Gear::EntitySystem::RegisterInActivateEntity(entityID);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::SelectedItem, ItemInfo::Bazooka);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::MyTurn, false);
		}

		m_ExternalVector->y -= 0.53f;

		if (m_ExternalVector->y < -30.0f)
		{
			m_ExternalVector->y = -30.0f;
		}
		if (m_ExternalVector->y > 0.0f || m_TargetPos->y < -20.0f)
		{
			return;
		}
		auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

		float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
		float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

		for (int i = -65; i <= 65; i += 5)
		{
			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i, ItemOnTexturePositionY - 60 });

			if (pixel == m_TargetPixelColor)
			{
				int yOffset = 60;
				while (1)
				{
					yOffset -= 2;
					pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i , ItemOnTexturePositionY - yOffset });
					if (pixel != m_TargetPixelColor)
					{
						float localY = (ItemOnTexturePositionY - yOffset - 1) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
						break;
					}
				}
				PLAY_SOUND_NAME("HOLYDONKEYIMPACT", WormsSound::Weapon);
				m_ExternalVector->y = -(m_ExternalVector->y * 0.7f);

				auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
				ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.3f), Explosion::Size100, 100.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y), Explosion::Text::Foom, "");
				
				break;
			}
		}
	}
}

