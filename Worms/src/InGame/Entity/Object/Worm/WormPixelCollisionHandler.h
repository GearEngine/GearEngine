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
			auto& m_ExternalVector = Gear::EntitySystem::GetPhysics2D(entityID)->GetExternalVector();
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (!(leftCollision ^ rightCollision))
			{
				if (dir == WormInfo::DirectionType::LeftUp || dir == WormInfo::DirectionType::LeftDown)
				{
					dir = WormInfo::DirectionType::LeftFlat;
				}
				else if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightDown)
				{
					dir = WormInfo::DirectionType::RightFlat;
				}
			}
			else if (leftCollision)
			{
				if (dir == WormInfo::DirectionType::RightUp || dir == WormInfo::DirectionType::RightFlat)
				{
					dir = WormInfo::DirectionType::RightDown;
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftDown)
				{
					dir = WormInfo::DirectionType::LeftUp;
				}
			}
			else if (rightCollision)
			{
				if (dir == WormInfo::DirectionType::RightDown || dir == WormInfo::DirectionType::RightFlat)
				{
					dir = WormInfo::DirectionType::RightUp;
				}
				else if (dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
				{
					dir = WormInfo::DirectionType::LeftDown;
				}
			}
			status->SetStat(WormInfo::Direction, dir);
		}

		inline virtual void Handle(int entityID) override
		{
			if (*m_ActivatedGravity == false)
			{
				return;
			}

			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto FSM = Gear::EntitySystem::GetFSM(entityID);

			if (m_TargetPos->y < -13.0f)
			{
				FSM->SetCurrentState(WormState::OnUnderWater);

				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnUnderWater);
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TurnOver)));
				physics->ActivateGravity();
				physics->SetPixelCollisionHandler("UnderWater");
				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			float wormOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float wormOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			//collision judge
			int basicYOffset = 8;
			int basicXOffset = 3;

			bool leftRightCollision = false;
			if (m_ExternalVector->x != 0.0f)
			{
				float tick = Gear::EntitySystem::GetTimer(entityID)->GetTick();
				int xOffset = 5;
				//right
				if (m_ExternalVector->x > 0.0f)
				{
					int yOffset = 5;
					bool isCollion = false;
					glm::vec3 rightPixel;
					auto rightPixelUp = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX + xOffset, (int)wormOnTexturePositionY + yOffset });
					if (rightPixelUp == m_TargetPixelColor)
					{
						isCollion = true;
					}
					auto leftDownPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX + xOffset, (int)wormOnTexturePositionY - yOffset });
					if (leftDownPixel == m_TargetPixelColor)
					{
						yOffset = -yOffset;
						isCollion = true;
					}

					if (isCollion)
					{
						int fixedXpos;
						for (int i = 1; i <= 10; ++i)
						{
							fixedXpos = (int)wormOnTexturePositionX + (xOffset - i);
							rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { fixedXpos, (int)wormOnTexturePositionY + yOffset });
							if (rightPixel != m_TargetPixelColor)
							{
								float localX = (fixedXpos) / m_TargetTextureWidth - 0.5f;
								m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
								m_ExternalVector->x = -m_ExternalVector->x * 0.9f;
								wormOnTexturePositionX -= (i * 0.5f);
								wormOnTexturePositionX = int(wormOnTexturePositionX + 0.5f);
								break;
							}
						}
					}
				}
				//left
				else
				{
					int yOffset = 5;
					bool isCollion = false;
					glm::vec3 leftPixel;
					auto leftPixelUp = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX - xOffset, (int)wormOnTexturePositionY + yOffset });
					if (leftPixelUp == m_TargetPixelColor)
					{
						isCollion = true;
					}
					auto leftDownPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX - xOffset, (int)wormOnTexturePositionY - yOffset });
					if (leftDownPixel == m_TargetPixelColor)
					{
						yOffset = -yOffset;
						isCollion = true;
					}

					if (isCollion)
					{
						int fixedXpos;
						for (int i = 1; i <= 10; ++i)
						{
							fixedXpos = (int)wormOnTexturePositionX - (xOffset - i);
							leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { fixedXpos, (int)wormOnTexturePositionY + yOffset });
							if (leftPixel != m_TargetPixelColor)
							{
								float localX = (fixedXpos) / m_TargetTextureWidth - 0.5f;
								m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
								m_ExternalVector->x = -m_ExternalVector->x * 0.9f;
								wormOnTexturePositionX += (i * 0.5f);
								wormOnTexturePositionX = int(wormOnTexturePositionX + 0.5f);
								break;
							}
						}
					}
				}
			}

			if (m_ExternalVector->y > 0.0f)
			{
				auto rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX, (int)wormOnTexturePositionY + basicYOffset });
				if (rightPixel == m_TargetPixelColor)
				{
					m_ExternalVector->y = 0.0f;
					m_ExternalVector->x = 0.0f;
					*m_GravityAccelation = 0.0f;
				}
			}

			//Position adjust
			int fixedBottomPos;
			auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX, (int)wormOnTexturePositionY - basicYOffset });
			bool myTurn = std::any_cast<bool>(status->GetStat(WormInfo::MyTurn));

			if (midPixel == m_TargetPixelColor)
			{
				auto curState = FSM->GetCurrentState();
				if (curState == WormState::OnSliding || curState == WormState::OnAttacked)
				{
					if (m_ExternalVector->y <= -20.f)
					{
						int fallenDamage = (int)((-m_ExternalVector->y - 20.0f) * 0.1f);
						if (myTurn)
						{
							Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TurnOver)));
							int selfDamage = std::any_cast<int>(status->GetStat(WormInfo::SelfDamage));
							selfDamage += fallenDamage;
							status->SetStat(WormInfo::SelfDamage, selfDamage);
						}
						else
						{
							int damage = std::any_cast<int>(status->GetStat(WormInfo::Damage));
							damage += fallenDamage;
							status->SetStat(WormInfo::Damage, damage);
						}
					}

					*m_GravityAccelation = 0.0f;
					m_ExternalVector->y = 0.0f;
					FSM->SetCurrentState(WormState::OnSliding);
					physics->SetPixelCollisionHandler("Sliding");
				}
				else
				{
					if (m_ExternalVector->y >= -20.f)
					{
						FSM->SetCurrentState(WormState::OnReadyLand);
					}
					else
					{
						int fallenDamage = (int)((-m_ExternalVector->y + 20.f) * 0.1f);
						GR_TRACE("Worm {0} First Damaged {1}", entityID, fallenDamage);
						if (myTurn)
						{
							Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TurnOver)));
							int selfDamage = std::any_cast<int>(status->GetStat(WormInfo::SelfDamage));
							selfDamage += fallenDamage;
							status->SetStat(WormInfo::SelfDamage, selfDamage);
						}
						else
						{
							int damage = std::any_cast<int>(status->GetStat(WormInfo::Damage));
							damage += fallenDamage;
							status->SetStat(WormInfo::Damage, damage);
						}
						FSM->SetCurrentState(WormState::OnReadyFallen);
					}
				}

				for (int i = 1; i < 40; ++i)
				{
					fixedBottomPos = (int)wormOnTexturePositionY - (basicYOffset - i);
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX, fixedBottomPos });
					if (midPixel != m_TargetPixelColor)
					{
						float localY = (wormOnTexturePositionY + i - 1) / m_TargetTextureHeight - 0.5f;
						m_TargetPos->y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
						physics->InActivateGravity();

						//State adjust
						auto leftBottomPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX - basicXOffset,  fixedBottomPos });
						auto rightBottomPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX + basicXOffset, fixedBottomPos });

						bool leftCollision = leftBottomPixel == m_TargetPixelColor ? true : false;
						bool rightCollision = rightBottomPixel == m_TargetPixelColor ? true : false;
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
					leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX - xOffset, midY + leftOffset });
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
				return Pixel::Gradient::Flat;
			}
			if (leftOffset > rightOffset)
			{
				return Pixel::Gradient::TopBottom;
			}
			if (leftOffset < rightOffset)
			{
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

			if (m_TargetPos->y < -13.0f)
			{
				FSM->SetCurrentState(WormState::OnUnderWater);
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TurnOver)));
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnUnderWater);
				physics->ActivateGravity();
				physics->SetPixelCollisionHandler("UnderWater");
				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

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

	class WormSlidingPCHandler : public Gear::Physics2D::PixelCollisionHander
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
					leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { midX - xOffset, midY + leftOffset });
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
				return Pixel::Gradient::Flat;
			}
			if (leftOffset > rightOffset)
			{
				return Pixel::Gradient::TopBottom;
			}
			if (leftOffset < rightOffset)
			{
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
			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			float wormOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float wormOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			if (m_TargetPos->y < -13.0f)
			{
				FSM->SetCurrentState(WormState::OnUnderWater);
				Gear::EntitySystem::GetAnimator2D(entityID)->PlayAnimation(WormState::OnUnderWater);
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TurnOver)));
				physics->ActivateGravity();
				physics->SetPixelCollisionHandler("UnderWater");
				*m_ExternalVector = { 0.0f, 0.0f };
				return;
			}

			//right
			int xOffset = 5;
			if (m_ExternalVector->x > 0.0f)
			{
				bool isCollion = false;
				auto rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX + xOffset, (int)wormOnTexturePositionY + 6 });
				isCollion = rightPixel == m_TargetPixelColor ? true : false;
				if (isCollion)
				{
					int fixedXpos;
					for (int i = 1; i <= 10; ++i)
					{
						fixedXpos = (int)wormOnTexturePositionX + (xOffset - i);
						rightPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { fixedXpos, (int)wormOnTexturePositionY + 6 });
						if (rightPixel != m_TargetPixelColor)
						{
							float localX = (fixedXpos) / m_TargetTextureWidth - 0.5f;
							m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
							m_ExternalVector->x = -m_ExternalVector->x * 0.95f;
							wormOnTexturePositionX -= (i * 0.5f);
							wormOnTexturePositionX = int(wormOnTexturePositionX + 0.5f);
							status->SetStat(WormInfo::Direction, WormInfo::DirectionType::LeftFlat);
							break;
						}
					}
				}
			}
			//left
			else
			{
				bool isCollion = false;
				auto leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)wormOnTexturePositionX - xOffset, (int)wormOnTexturePositionY + 4 });
				isCollion = leftPixel == m_TargetPixelColor ? true : false;
				if (isCollion)
				{
					int fixedXpos;
					for (int i = 1; i <= 10; ++i)
					{
						fixedXpos = (int)wormOnTexturePositionX - (xOffset - i);
						leftPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { fixedXpos, (int)wormOnTexturePositionY + 4});
						if (leftPixel != m_TargetPixelColor)
						{
							float localX = (fixedXpos) / m_TargetTextureWidth - 0.5f;
							m_TargetPos->x = (*m_PixelCollisionTargetTextureTranslate)[0][0] * localX + (*m_PixelCollisionTargetTextureTranslate)[3][0];
							m_ExternalVector->x = -m_ExternalVector->x * 0.95f;
							wormOnTexturePositionX += (i * 0.5f);
							wormOnTexturePositionX = int(wormOnTexturePositionX + 0.5f);
							status->SetStat(WormInfo::Direction, WormInfo::DirectionType::RightFlat);
							break;
						}
					}
				}
			}

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
				}
			}
		}
	};

	class WormOnUnderWater : public Gear::Physics2D::PixelCollisionHander
	{
		inline virtual void Handle(int entityID) override
		{
			m_ExternalVector->y = -1.3f;
		}
	};
}