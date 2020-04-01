#pragma once

#include "WormEnum.h"

namespace InGame {

	namespace Pixel {

		enum Gradient : unsigned int
		{
			Flat,
			BottomTop,
			TopBottom
		};
	}

	class WormOnAirPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		inline void StateHandle(int entityID, bool leftCollision, bool rightCollision)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);

			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
			bool onMyTurn = std::any_cast<bool>(status->GetStat(WormInfo::MyTurn));

			if (onMyTurn)
			{
				FSM->SetCurrentState(WormState::OnBreath);
				physics->SetPixelCollisionHandler("Move");
			}

			if (!(leftCollision ^ rightCollision))
			{
				if (dir == WormInfo::DirectionType::LeftUp || dir == WormInfo::DirectionType::LeftDown)
				{
					dir = WormInfo::DirectionType::LeftFlat;
					status->SetStat(WormInfo::Direction, dir);
				}
				else if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightDown)
				{
					dir = WormInfo::DirectionType::RightFlat;
					status->SetStat(WormInfo::Direction, dir);
				}
			}
			else if (leftCollision)
			{
				if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightFlat)
				{
					dir = WormInfo::DirectionType::RightDown;
					status->SetStat(WormInfo::Direction, dir);
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftDown)
				{
					dir = WormInfo::DirectionType::LeftUp;
					status->SetStat(WormInfo::Direction, dir);
				}
			}
			else if (rightCollision)
			{
				if (dir == WormInfo::DirectionType::RightDown || dir == WormInfo::DirectionType::RightFlat)
				{
					dir = WormInfo::DirectionType::RightUp;
					status->SetStat(WormInfo::Direction, dir);
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					dir = WormInfo::DirectionType::LeftDown;
					status->SetStat(WormInfo::Direction, dir);
				}
			}
			switch (dir)
			{
			case WormInfo::DirectionType::LeftFlat:
				animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
				break;
			case WormInfo::DirectionType::LeftUp:
				animator->SetCurrentAnimation(WormState::OnLeftUpBreath);
				break;
			case WormInfo::DirectionType::LeftDown:
				animator->SetCurrentAnimation(WormState::OnLeftDownBreath);
				break;
			case WormInfo::DirectionType::RightFlat:
				animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
				break;
			case WormInfo::DirectionType::RightUp:
				animator->SetCurrentAnimation(WormState::OnRightUpBreath);
				break;
			case WormInfo::DirectionType::RightDown:
				animator->SetCurrentAnimation(WormState::OnRightDownBreath);
				break;
			}
			animator->ResumeAnimation();
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
			int basicXOffset = 4;
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
		Pixel::Gradient GradientCheck(int midX, int midY)
		{

			int xOffset = 4;
			int gradRange = 4;

			auto leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX - xOffset, midY });
			auto rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX + xOffset, midY });
		
			int leftOffset = 0;
			if (leftPixel == m_TargetPixelColor)
			{
				for (int i = 0; i < gradRange; ++i)
				{
					++leftOffset;
					leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX - xOffset, midY + leftOffset});
					if (leftPixel != m_TargetPixelColor)
					{
						--leftOffset;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < gradRange; ++i)
				{
					--leftOffset;
					leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX - xOffset , midY + leftOffset });
					if (leftPixel == m_TargetPixelColor)
					{
						break;
					}
				}
			}

			int rightOffset = 0;
			if (rightPixel == m_TargetPixelColor)
			{
				for (int i = 0; i < gradRange; ++i)
				{
					++rightOffset;
					rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX + xOffset, midY + rightOffset });
					if (rightPixel != m_TargetPixelColor)
					{
						--rightOffset;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < gradRange; ++i)
				{
					--rightOffset;
					rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX + xOffset, midY + rightOffset });
					if (rightPixel == m_TargetPixelColor)
					{
						break;
					}
				}
			}
			//result
			if (leftOffset == rightOffset)
			{
				GR_TRACE("Grad Flat");
				return Pixel::Gradient::Flat;
			}
			if (leftOffset > rightOffset)
			{
				GR_TRACE("Grad TopBottom");
				return Pixel::Gradient::TopBottom;
			}
			if (leftOffset < rightOffset)
			{
				GR_TRACE("Grad BottomTop");
				return Pixel::Gradient::BottomTop;
			}
		}

		void StateHande(int entityID, Pixel::Gradient gradient)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
			switch (gradient)
			{
			case InGame::Pixel::Flat:
				if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftUp | dir == WormInfo::DirectionType::LeftFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
				}
				else
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
				}
				break;
			case InGame::Pixel::BottomTop:
				if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftUp | dir == WormInfo::DirectionType::LeftFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftDown);
				}
				else
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightUp);
				}
				break;
			case InGame::Pixel::TopBottom:
				if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftUp | dir == WormInfo::DirectionType::LeftFlat)
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftUp);
				}
				else
				{
					status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightDown);
				}
				break;
			}
		}

		inline virtual void Handle(int entityID) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			float wormOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float wormOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			int basicYOffset = 8;

			auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX, (int)wormOnTexturePositionY - basicYOffset });
			bool midCollision = midPixel == m_TargetPixelColor ? true : false;

			if (!midCollision)
			{
				int bottomDist = 0;
				int fixedBottomPos;

				while (++bottomDist)
				{
					fixedBottomPos = ((int)wormOnTexturePositionY - basicYOffset) - bottomDist;
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX, fixedBottomPos });
					if (midPixel == m_TargetPixelColor)
					{
						//Position adjust
						float localY = (fixedBottomPos + basicYOffset) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];

						//Slope Check
						auto gradient = GradientCheck(wormOnTexturePositionX, fixedBottomPos);
						StateHande(entityID, gradient);
						break;
					}
					//On Air
					if (bottomDist == 10)
					{
						FSM->SetCurrentState(WormState::OnAir);
						physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
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
					fixedBottomPos = ((int)wormOnTexturePositionY - basicYOffset) + bottomDist;
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX, fixedBottomPos });
					if (midPixel != m_TargetPixelColor)
					{
						--fixedBottomPos;
						//Position adjust
						float localY = (fixedBottomPos + basicYOffset) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];

						//Slope Check
						auto gradient = GradientCheck(wormOnTexturePositionX, fixedBottomPos);
						StateHande(entityID, gradient);
						break;
					}
					//block the wall
					if (bottomDist == 10)
					{
						float moveSpeed = std::any_cast<float>(status->GetStat(WormInfo::MoveSpeed));
						auto wormDir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
						float tick = timer->GetTick();

						if (wormDir == WormInfo::DirectionType::RightDown || wormDir == WormInfo::DirectionType::RightUp || wormDir == WormInfo::DirectionType::RightFlat)
						{
							m_TargetPos->x -= moveSpeed * tick;
						}
						else
						{
							m_TargetPos->x += moveSpeed * tick;
						}
						Gear::EntitySystem::GetAnimator2D(entityID)->StopAnimation();
						break;
					}
				}
			}
		}
	};
}