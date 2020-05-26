#pragma once

#include "../ItemEnum.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"

namespace InGame {

	class BabyHosOnGoingHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Animator2D> animator;
		int rotateDir;

		bool r_Down;
		bool g_Down;
		bool b_Down;

		float rgbChangeRatio = 0.02f;

		float exhaustGenDelay = 0.03f;
		float exhaustPastTime = 0.0f;

		virtual void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			
			r_Down = Gear::Util::GetRndInt(2);
			g_Down = Gear::Util::GetRndInt(2);
			b_Down = Gear::Util::GetRndInt(2);

			rotateDir = std::any_cast<int>(status->GetStat(Item::Info::RotateDir));
			OnAwake = false;
		}

		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

	class BabyHosOnUnderWater : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::FrameTexture2D> missileTexture;
		Gear::Ref<Gear::Status> Status;
		ItemInfo::Number number;

		bool drawnSound = false;
		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			missileTexture = Gear::TextureStorage::GetFrameTexture2D("BabyHosBullet");
			Status = Gear::EntitySystem::GetStatus(entityID);

			number = std::any_cast<ItemInfo::Number>(Status->GetStat(Item::Info::Number));

			if (number != ItemInfo::BabyHos)
			{
				animator->StopAnimation();
			}
			else
			{
				animator->GetCurrentAnimation()->GetTintColor().a = 0.5f;
			}
			OnAwake = false;
		}

		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			if (!drawnSound)
			{
				drawnSound = true;
				PLAY_SOUND_NAME("Splash", WormsSound::Water);
			}

			if (number != ItemInfo::BabyHos)
			{
				Gear::Renderer2D::DrawFrameQuad(transform->GetTranslate(), missileTexture, 0, 0, glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			}

			if (transform->GetPosition().y < -19.0f)
			{
				drawnSound = false;
				Gear::EntitySystem::RegisterInActivateEntity(entityID);
			}
			return Item::State::OnUnderWater;
		}
	};

	class BabyHosOnExplosion : public Gear::FSM::InputHandler
	{
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};
}
