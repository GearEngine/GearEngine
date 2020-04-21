#pragma once

#include "../ItemEnum.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"

namespace InGame {

	class BabyBananaOnGoingHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::Physics2D> physics;

		virtual void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			physics = Gear::EntitySystem::GetPhysics2D(entityID);
			OnAwake = false;
		}

		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			float& angle = transform->GetRotation();
			auto& externalVector = physics->GetExternalVector();
			float externalVectorPower = std::abs(externalVector.x) + std::abs(externalVector.y);
			angle -= externalVectorPower * timer->GetTick() * 0.8f;

			return Item::State::OnGoing;
		}
	};

	class BabyBananaOnUnderWater : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::FrameTexture2D> missileTexture;

		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			missileTexture = Gear::TextureStorage::GetFrameTexture2D("BananaBullet");
			animator->StopAnimation();
		}

		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			Gear::Renderer2D::DrawFrameQuad(transform->GetTranslate(), missileTexture, 0, 31, glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));

			if (transform->GetPosition().y < -19.0f)
			{
				Gear::EntitySystem::RegisterInActivateEntity(entityID);
			}
			return Item::State::OnUnderWater;
		}
	};

	class BabyBananaOnExplosion : public Gear::FSM::InputHandler
	{
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

}