#pragma once

#include "CameraEnum.h"

namespace InGame {

	class CameraOnFlowingHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);

			return CameraState::OnFlowing;
		}
	};

	class CameraOnStopHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			if (timer->isExpired())
			{
				return CameraState::OnFlowing;
			}

			return CameraState::OnStop;
		}

	};

}