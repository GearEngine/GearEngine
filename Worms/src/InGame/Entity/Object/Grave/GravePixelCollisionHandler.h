#pragma once

#include "GraveFSMHandler.h"

namespace InGame {

	class GraveGenerallPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Transform2D> transform;
		bool inFirst = true;
		float elastics;

		void init(int entityID)
		{
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			elastics = physics->GetElastics();

			inFirst = false;
		}

		virtual void Handle(int entityID) override
		{
			if (inFirst)
			{
				init(entityID);
				return;
			}
			if (!physics->IsActivateGravity())
			{
				return;
			}
			auto& externalVectorY = physics->GetExternalVectorY();
			auto& position = transform->GetPosition();

			if (position.y < -13.0f)
			{
				FSM->SetCurrentState(GraveInfo::State::OnUnderWater);
				int curFrameY = animator->GetFrameIdx().second;

				animator->SetCurrentAnimation(GraveInfo::OnUnderWater);
				animator->SetFrameY(curFrameY);
				animator->ResumeAnimation();

				physics->ActivateGravity();
				physics->SetPixelCollisionHandler("UnderWater");
				physics->SetExternalVectorY(0.0f);
				return;
			}

			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(glm::vec2(position.x, position.y), *m_PixelCollisionTargetTextureTranslate);

			float GraveOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float GraveOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;

			//Position adjust

			if (externalVectorY > 0.0f)
			{
				int basicYOffset = 8;
				int fixedBottomPos;
				auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)GraveOnTexturePositionX, (int)GraveOnTexturePositionY + basicYOffset });

				if (midPixel == m_TargetPixelColor)
				{
					PLAY_SOUND_NAME("CrateImpact", WormsSound::Grave);
					for (int i = 1; i < 40; ++i)
					{
						fixedBottomPos = (int)GraveOnTexturePositionY + (basicYOffset - i);
						midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)GraveOnTexturePositionX, fixedBottomPos });
						if (midPixel != m_TargetPixelColor)
						{
							float localY = (GraveOnTexturePositionY + i - 1) / m_TargetTextureHeight - 0.5f;
							position.y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];

							externalVectorY = 0.0f;
							physics->ResetGravityAccelation();
							break;
						}
					}
				}
			}
			else
			{
				int basicYOffset = 4;
				int fixedBottomPos;
				auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)GraveOnTexturePositionX, (int)GraveOnTexturePositionY - basicYOffset });

				if (midPixel == m_TargetPixelColor)
				{
					if (std::abs(m_ExternalVector->y) > 0.1f)
					{
						PLAY_SOUND_NAME("CrateImpact", WormsSound::Grave);
					}
					for (int i = 1; i < 40; ++i)
					{
						fixedBottomPos = (int)GraveOnTexturePositionY - (basicYOffset - i);
						midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)GraveOnTexturePositionX, fixedBottomPos });
						if (midPixel != m_TargetPixelColor)
						{
							float localY = (GraveOnTexturePositionY + i - 1) / m_TargetTextureHeight - 0.5f;
							position.y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
							if (externalVectorY < -0.01f)
							{
								externalVectorY = -externalVectorY * elastics;
								physics->ResetGravityAccelation();
							}
							else
							{
								physics->InActivateGravity();
								externalVectorY = 0.0f;
							}
							break;
						}
					}
				}
			}
		}

	};

	class GraveOnUnderWaterPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		virtual void Handle(int entityID) override
		{
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVectorY(-0.05f);
		}
	};
}