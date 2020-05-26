#pragma once
#include "../ItemEnum.h"

namespace InGame {

	class HosOnGoingHandler : public Gear::FSM::InputHandler
	{
		bool awake = false;
		Gear::Ref<Gear::Transform2D> Transform;
		inline void Awake(int entityID) override
		{
			awake = true;
			Transform = Gear::EntitySystem::GetTransform2D(entityID);
		}
		Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

}