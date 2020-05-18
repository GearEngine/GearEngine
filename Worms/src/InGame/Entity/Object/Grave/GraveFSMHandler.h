#pragma once
#include "GraveEventHandler.h"

namespace InGame {

	class GraveOnNothingHandler : public Gear::FSM::InputHandler
	{
		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return GraveInfo::State::OnNothing;
		}
	};

	class GraveOnUnderWaterHandler : public Gear::FSM::InputHandler
	{
		bool drawnSound = false;
		bool awake = true;
		Gear::Ref<Gear::Transform2D> transform;

		void Awake(int entityID)
		{
			transform = Gear::EntitySystem::GetTransform2D(entityID);
		}

		inline Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (awake)
			{
				Awake(entityID);
				awake = false;
			}
			if (!drawnSound)
			{
				drawnSound = true;
				PLAY_SOUND_NAME("Splash", WormsSound::Water);
			}

			if (transform->GetPosition().y < -19.0f)
			{
				drawnSound = false;
				Gear::EntitySystem::RegisterInActivateEntity(entityID);
			}

			return GraveInfo::State::OnUnderWater;
		}
	};

}