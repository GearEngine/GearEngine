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

	void HosPCHandler::BoundHandle(int entityID)
	{
		glm::vec3 leftPixel;
		glm::vec3 rightPixel;

		int leftDy = 5;
		int rightDy = 5;
		for (leftDy; leftDy >= -5; --leftDy)
		{
			leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { collisionXPoint - 2, collisionYPoint + leftDy });
			if (leftPixel == m_TargetPixelColor)
			{
				break;
			}
		}
		for (rightDy; rightDy >= -5; --rightDy)
		{
			rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { collisionXPoint + 2, collisionYPoint + rightDy });
			if (rightPixel == m_TargetPixelColor)
			{
				break;
			}
		}

		float inclination = (rightDy - leftDy) / 5.0f;
		m_ExternalVector->y = (-m_ExternalVector->y * 0.9f);
		if (inclination == 0.0f)
		{
			return;
		}
		float orthogonalinclination = -1.0f / inclination;
		m_ExternalVector->x = orthogonalinclination * 2.0f;
	}

	void HosPCHandler::Handle(int entityID)
	{
		if (m_TargetPos->y < -130.0f)
		{
			auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
			ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.7f), Explosion::Size25, 0.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y), Explosion::Text::Null, "None");

			PLAY_SOUND_NAME("YOUARENOTAVOID", WormsSound::Hos);
			STOP_SOUND_CAHNNEL(WormsSound::bgm);
			Gear::SoundSystem::Get()->SetVolue(WormsSound::bgm, 0.5f);

			Gear::EntitySystem::RegisterInActivateEntity(entityID);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::SelectedItem, ItemInfo::Bazooka);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::MyTurn, false);

			sendEventNoFollow = false;
		}

		m_ExternalVector->y -= 0.53f;

		if (m_ExternalVector->y < -30.0f)
		{
			m_ExternalVector->y = -30.0f;
		}
		if (m_TargetPos->y < -20.0f)
		{
			if (!sendEventNoFollow)
			{
				sendEventNoFollow = true;
				int cameraID = Gear::EntitySystem::GetEntityIDFromName("Camera");
				Gear::EventSystem::DispatchEventTo(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow)), cameraID);
			}
			return;
		}
		auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

		float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
		float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

		bool leftCollision = false;
		bool rightCollision = false;
		
		int hosRadius = 45;

		if (m_ExternalVector->x > 0.0f)
		{
			for (int i = -hosRadius; i <= hosRadius; i += 5)
			{
				int xOffset = hosRadius;
				auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + xOffset, ItemOnTexturePositionY + i });
				if (pixel == m_TargetPixelColor)
				{
					while (1)
					{
						xOffset -= 2;
						pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + xOffset, ItemOnTexturePositionY + i });
						if (pixel != m_TargetPixelColor)
						{
							float localX = (ItemOnTexturePositionX + xOffset) / m_TargetTextureWidth - 0.5f;
							m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
							break;
						}
					}
					//PLAY_SOUND_NAME("HOLYDONKEYIMPACT", WormsSound::Weapon);

					auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
					ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x + 1.0f, m_TargetPos->y), Explosion::Size100, 100.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x + 1.0f, m_TargetPos->y), Explosion::Text::Foom, "");
					Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnExplosion);
					rightCollision = true;
					break;
				}
			}
		}
		else if (m_ExternalVector->x < 0.0f)
		{
			for (int i = -hosRadius; i <= hosRadius; i += 5)
			{
				int xOffset = hosRadius;
				auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX - xOffset, ItemOnTexturePositionY + i });
				if (pixel == m_TargetPixelColor)
				{
					while (1)
					{
						xOffset -= 2;
						pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX - xOffset , ItemOnTexturePositionY + i });
						if (pixel != m_TargetPixelColor)
						{
							float localX = (ItemOnTexturePositionX - xOffset) / m_TargetTextureWidth - 0.5f;
							m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
							break;
						}
					}
					//PLAY_SOUND_NAME("HOLYDONKEYIMPACT", WormsSound::Weapon);

					auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
					ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x - 1.0f, m_TargetPos->y), Explosion::Size100, 100.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x - 1.0f, m_TargetPos->y), Explosion::Text::Foom, "");
					Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnExplosion);
					leftCollision = true;
					break;
				}
			}
		}
		if (leftCollision || rightCollision)
		{
			m_ExternalVector->x = -m_ExternalVector->x;
			if (m_ExternalVector->y < 0.0f)
			{
				m_ExternalVector->y = 15.0f;
			}
			return;
		}


		for (int i = -hosRadius; i <= hosRadius; i += 5)
		{
			int yOffset = hosRadius;
			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i, ItemOnTexturePositionY - yOffset });
			if (pixel == m_TargetPixelColor)
			{
				while (1)
				{
					yOffset -= 2;
					pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i , ItemOnTexturePositionY - yOffset });
					if (pixel != m_TargetPixelColor)
					{
						collisionXPoint = ItemOnTexturePositionX + i;
						collisionYPoint = ItemOnTexturePositionY - yOffset - 1;
						BoundHandle(entityID);
						float localY = (ItemOnTexturePositionY - yOffset - 1) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
						break;
					}
				}
				//PLAY_SOUND_NAME("HOLYDONKEYIMPACT", WormsSound::Weapon);

				auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
				ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.3f), Explosion::Size100, 100.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.3f), Explosion::Text::Foom, "");
				Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(Item::State::OnExplosion);

				break;
			}
		}
	}
}

