#pragma once

namespace InGame {


	class WormGenerallPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		inline virtual void Handle(int entityID) override
		{
			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto tick = Gear::EntitySystem::GetTimer(entityID)->GetTick();
			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			int realPositionX = textureLocalPosition.x * m_TargetTextureWidth;
			int realPositionY = textureLocalPosition.y * m_TargetTextureHeight;

			int basicYOffset = 8;
			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { realPositionX, realPositionY - basicYOffset });
			if (pixel == m_TargetPixelColor)
			{
				*m_GravityAccelation = 0.0f;
				m_ExternalVector->y = 0.0f;
			}

			for (int i = 1; i < basicYOffset; ++i)
			{
				pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { realPositionX, realPositionY - (basicYOffset - i) });
				if (pixel != m_TargetPixelColor)
				{
				
					break;
				}
			}
		}
	};


	class WormLeftMovePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		inline virtual void Handle(int entityID) override
		{
			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto textureLocalPosition = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			int realPositionX = textureLocalPosition.x * m_TargetTextureWidth;
			int realPositionY = textureLocalPosition.x * m_TargetTextureHeight;

			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { realPositionX, realPositionY });
		}
	};

	class WormRightMovePCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		inline virtual void Handle(int entityID) override
		{
			auto FSM = Gear::EntitySystem::GetFSM(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto textureLocalPosition = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(*m_TargetPos, *m_PixelCollisionTargetTextureTranslate);

			int realPositionX = textureLocalPosition.x * m_TargetTextureWidth;
			int realPositionY = textureLocalPosition.x * m_TargetTextureHeight;

			auto pixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { realPositionX, realPositionY });

		}
	};





}