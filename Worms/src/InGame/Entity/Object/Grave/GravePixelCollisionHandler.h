#pragma once

#include "GraveFSMHandler.h"

namespace InGame {

	class GraveGenerallPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Transform2D> transform;
		bool inFirst = true;
		float elastics;

		void init(int entityID)
		{
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
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
			if (externalVectorY > 0.0f)
			{
				return;
			}

			auto trans = transform->GetTranslate();
			auto TargetPos = glm::vec2(trans[3][0], trans[3][1]);

			if (TargetPos.y < -13.0f)
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

			auto textureLocalPosition = s_CoordManager->GetTextureLocalPosition_From_WorlPosition(TargetPos, *m_PixelCollisionTargetTextureTranslate);
			
			float GraveOnTexturePositionX = textureLocalPosition.x * m_TargetTextureWidth;
			float GraveOnTexturePositionY = textureLocalPosition.y * m_TargetTextureHeight;			

			//Position adjust
			int basicYOffset = 4;
			int fixedBottomPos;
			auto midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)GraveOnTexturePositionX, (int)GraveOnTexturePositionY - basicYOffset });

			if (midPixel == m_TargetPixelColor)
			{
				PLAY_SOUND_NAME("CrateImpact", WormsSound::Grave);
				for (int i = 1; i < 40; ++i)
				{
					fixedBottomPos = (int)GraveOnTexturePositionY - (basicYOffset - i);
					midPixel = s_CoordManager->GetPixel_From_TextureLocal_With_TextureRealPosition(m_TargetTexture, { (int)GraveOnTexturePositionX, fixedBottomPos });
					if (midPixel != m_TargetPixelColor)
					{
						float localY = (GraveOnTexturePositionY + i - 1) / m_TargetTextureHeight - 0.5f;
						TargetPos.y = (*m_PixelCollisionTargetTextureTranslate)[1][1] * localY + (*m_PixelCollisionTargetTextureTranslate)[3][1];
						
						
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

	};

	class GraveOnUnderWaterPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		virtual void Handle(int entityID) override
		{
			Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVectorY(-0.05f);
		}
	};
}