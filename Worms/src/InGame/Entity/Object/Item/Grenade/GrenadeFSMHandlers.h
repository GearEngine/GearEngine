#pragma once

#include "../ItemEnum.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"

namespace InGame {

	class GrenadeOnGoingHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Timer> timer;
		
		virtual void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			OnAwake = false;
		}

		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			if (timer->isExpired())
			{
				return Item::State::OnExplosion;
			}
			return Item::State::OnGoing;
		}
	};

	class GrenadeOnUnderWater : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::FrameTexture2D> missileTexture;

		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			missileTexture = Gear::TextureStorage::GetFrameTexture2D("GrenadeBullet");
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
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
				Gear::EntitySystem::RegisterInActivateEntity(entityID);
			}
			return Item::State::OnUnderWater;
		}
	};

	class GrenadeOnExplosion : public Gear::FSM::InputHandler
	{
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

}