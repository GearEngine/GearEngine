#pragma once

#include "WormEnum.h"

namespace InGame {

	namespace WormPixelCollision {

		

	}


	class WormOnAirPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		inline void StateHandle(int entityID, bool leftCollision, bool rightCollision)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);

			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (!(leftCollision ^ rightCollision))
			{
				if (dir == WormInfo::DirectionType::LeftUp || dir == WormInfo::DirectionType::LeftDown)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
				}
				else if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightDown)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
				}
			}
			else if (leftCollision)
			{
				if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightDown);
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftDown)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftUp);
				}
			}
			else if (rightCollision)
			{
				if (dir == WormInfo::DirectionType::RightDown || dir == WormInfo::DirectionType::RightFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightUp);
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftDown);
				}
			}
		}
		
		inline virtual void Handle(int entityID) override
		{
			if (*m_ActivatedGravity == false)
			{
				return;
			}

			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			float realPositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float realPositionY = textureLocalPosition.y * m_TargetTextureHeight;

			int basicYOffset = 8;
			int basicXOffset = 3;
			auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX, (int)realPositionY - basicYOffset });

			//Position adjust
			int fixedBottomPos;
			if (midPixel == m_TargetPixelColor)
			{
				*m_GravityAccelation = 0.0f;
				m_ExternalVector->x = 0.0f;
				m_ExternalVector->y = 0.0f;
				for (int i = 1; i < 50; ++i)
				{
					fixedBottomPos = (int)realPositionY - (basicYOffset - i);
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX, fixedBottomPos });
					if (midPixel != m_TargetPixelColor)
					{
						float localY = (realPositionY + i - 1) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
						physics->InActivateGravity();

						//State adjust
						auto leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX - basicXOffset,  fixedBottomPos });
						auto rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX + basicXOffset, fixedBottomPos });

						bool leftCollision = leftPixel == m_TargetPixelColor ? true : false;
						bool rightCollision = rightPixel == m_TargetPixelColor ? true : false;
						StateHandle(entityID, leftCollision, rightCollision);
						break;
					}
				}
			}
		}
	};

	class WormMovePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		inline void StateHandle(int entityID, bool leftCollision, bool rightCollision)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);

			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (!(leftCollision ^ rightCollision))
			{
				if (dir == WormInfo::DirectionType::LeftUp || dir == WormInfo::DirectionType::LeftDown)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
				}
				else if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightDown)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
				}
			}
			else if (leftCollision)
			{
				if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightDown);
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftDown)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftUp);
				}
			}
			else if (rightCollision)
			{
				if (dir == WormInfo::DirectionType::RightDown || dir == WormInfo::DirectionType::RightFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightUp);
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftDown);
				}
			}
		}

		inline virtual void Handle(int entityID) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto textureLocalPosition = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			float realPositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float realPositionY = textureLocalPosition.y * m_TargetTextureHeight;

			int basicYOffset = 8;
			int basicXOffset = 3;

			auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX, (int)realPositionY - basicYOffset });
			auto leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX - basicXOffset, (int)realPositionY - basicYOffset });
			auto rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX + basicXOffset, (int)realPositionY - basicYOffset });

			bool midCollision = midPixel == m_TargetPixelColor ? true : false;
			bool leftCollision = leftPixel == m_TargetPixelColor ? true : false;
			bool rightCollision = rightPixel == m_TargetPixelColor ? true : false;

			if (!midCollision)
			{
				int bottomDist = 0;
				int fixedBottomPos;

				while (++bottomDist)
				{
					fixedBottomPos = ((int)realPositionY - basicYOffset) - bottomDist;
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX, fixedBottomPos });
					if (midPixel == m_TargetPixelColor)
					{
						//Position adjust
						float localY = (fixedBottomPos + basicYOffset) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];

						//State adjust
						leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX - basicXOffset, fixedBottomPos });
						rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX + basicXOffset, fixedBottomPos });

						leftCollision = leftPixel == m_TargetPixelColor ? true : false;
						rightCollision = rightPixel == m_TargetPixelColor ? true : false;

						StateHandle(entityID, leftCollision, rightCollision);
						break;
					}
					//On Air
					if (bottomDist == 20)
					{
						physics->ActivateGravity();
						physics->SetPixelCollisionHandler("OnAir");
						return;
					}
				}
			}
			else
			{
				int fixedBottomPos;
				int bottomDist = 0;
				while (++bottomDist)
				{
					fixedBottomPos = ((int)realPositionY - basicYOffset) + bottomDist;
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX, fixedBottomPos });
					if (midPixel != m_TargetPixelColor)
					{
						//Position adjust
						float localY = (fixedBottomPos + basicYOffset - 1) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];

						//State adjust
						leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX - basicXOffset, fixedBottomPos });
						rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)realPositionX + basicXOffset, fixedBottomPos });

						leftCollision = leftPixel == m_TargetPixelColor ? true : false;
						rightCollision = rightPixel == m_TargetPixelColor ? true : false;

						break;
					}
					//block the wall
					if (bottomDist == 20)
					{
						float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::MoveSpeed));
						float tick = timer->GetTick();

						/*if()
						m_TargetPos->x += moveSpeed * tick;*/

						break;
					}
				}
				StateHandle(entityID, leftCollision, rightCollision);
			}
		}
	};
}