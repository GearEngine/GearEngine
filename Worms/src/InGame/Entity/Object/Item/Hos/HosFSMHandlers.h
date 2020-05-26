#pragma once
#include "../ItemEnum.h"

namespace InGame {

	class HosOnGoingHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Transform2D> Transform;
		Gear::Ref<Gear::Timer> timer;
		inline void Awake(int entityID) override
		{
			OnAwake = false;
			Transform = Gear::EntitySystem::GetTransform2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
		}
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

	class HosOnExplosionHandler : public Gear::FSM::InputHandler
	{
		Gear::Ref<Gear::Transform2D> Transform;
		Gear::Ref<Gear::Status> Status;

		inline void Awake(int entityID) override
		{
			OnAwake = false;
			Transform = Gear::EntitySystem::GetTransform2D(entityID);
			Status = Gear::EntitySystem::GetStatus(entityID);
		}
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};
}