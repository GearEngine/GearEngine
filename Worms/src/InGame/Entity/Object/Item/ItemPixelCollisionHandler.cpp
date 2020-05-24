#include "wmpch.h"
#include "ItemPixelCollisionHandler.h"

#include "Item.h"

namespace InGame {

	void InGame::DonkeyTypePCHandler::Handle(int entityID)
	{
		if (m_TargetPos->y < -70.0f)
		{
			Gear::EntitySystem::RegisterInActivateEntity(entityID);
			int from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::SelectedItem, ItemInfo::Bazooka);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::MyTurn, false);
			Gear::EntitySystem::GetFSM(from)->SetCurrentState(WormState::OnNotMyTurn);
			
			if (collisionCnt == 0)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
			}
			collisionCnt = 0;
		}

		m_ExternalVector->y -= 0.53f;

		if (m_ExternalVector->y < -30.0f)
		{
			m_ExternalVector->y = -30.0f;
		}
		if (m_ExternalVector->y > 0.0f)
		{
			return;
		}
		auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

		float ItemOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
		float ItemOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;


		for (int i = -75; i <= 75; i += 5)
		{
			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { ItemOnTexturePositionX + i, ItemOnTexturePositionY - 70 });

			if (pixel == m_TargetPixelColor)
			{
				PLAY_SOUND_NAME("HOLYDONKEYIMPACT", WormsSound::Weapon);
				collisionCnt++;
				m_ExternalVector->y = -(m_ExternalVector->y * 0.7f);

				auto from = std::any_cast<int>(Gear::EntitySystem::GetStatus(entityID)->GetStat(Item::Info::From));
				ITEM_POOL->MakeExplosion(ExplosionData(glm::vec2(m_TargetPos->x, m_TargetPos->y - 1.7f), Explosion::Size100, 100.0f, ItemInfo::Donkey, from), glm::vec2(m_TargetPos->x, m_TargetPos->y), Explosion::Text::Foom, "");
			}
			break;
		}
		
	}

}

