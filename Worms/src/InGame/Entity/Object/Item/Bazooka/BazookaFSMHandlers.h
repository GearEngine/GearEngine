#pragma once

#include "../ItemEnum.h"

namespace InGame {

	class BazookaOnGoingHandler : public Gear::FSM::InputHandler
	{
	public:
		const float m_GenExhaustDelay = 0.03f;
		float m_pastTime = 0.0f;
		
		Gear::Ref<Gear::Physics2D> physics;
		Gear::Ref<Gear::Animator2D> animator;

		virtual void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			physics = Gear::EntitySystem::GetPhysics2D(entityID); 
			OnAwake = false;
			WindAdjustRatio = 0.7f;
		}
	private:
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
		float WindAdjustRatio;
	};

	class BazookaOnUnderWater : public Gear::FSM::InputHandler
	{

		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Transform2D> transform;
		Gear::Ref<Gear::FrameTexture2D> missileTexture;

		inline void Awake(int entityID) override
		{
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			transform = Gear::EntitySystem::GetTransform2D(entityID);
			missileTexture = Gear::TextureStorage::GetFrameTexture2D("BazukaBullet");
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

	class BazookaOnExplosion : public Gear::FSM::InputHandler
	{
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};
}