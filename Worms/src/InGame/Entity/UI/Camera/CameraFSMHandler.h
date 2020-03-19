#pragma once

#include "CameraEnum.h"

namespace InGame {

	class CameraOnFlowingHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);

			if (cmd.KeyType == CameraCommand::MouseMove)
			{
				physics->SetExternalVector(cmd.MouseMove);
				return CameraState::OnMoveWithMouse;
			}

			return CameraState::OnFlowing;
		}
	};

	class CameraOnMoveWithMouseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			
			if (cmd.KeyType == CameraCommand::MouseMove)
			{
				physics->SetExternalVector(cmd.MouseMove);
				return CameraState::OnMoveWithMouse;
			}

			physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			timer->SetTimer(5.0f);
			timer->Start();
			return CameraState::OnStop;
		}
	};

	class CameraOnStopHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			if (timer->isExpired())
			{
				return CameraState::OnFlowing;
			}

			if (cmd.KeyType == CameraCommand::MouseMove)
			{
				physics->SetExternalVector(cmd.MouseMove);
				return CameraState::OnMoveWithMouse;
			}

			return CameraState::OnStop;
		}

	};

}