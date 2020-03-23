#pragma once
#include "InGame/Data/EventData.h"

namespace InGame {

	class CameraExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			GR_TRACE("No.{0} Entity get explosion event!", entityID);
			
		}
	};

	class CameraMouseMoveEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			auto mouseMoveData = std::any_cast<MouseMoveData>(data);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			physics->PauseFollowingTarget();

			physics->SetExternalVector(glm::vec2(mouseMoveData.dx, mouseMoveData.dy));
		}
	};

	class CameraWorldEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewFollow)
			{
				if (worldData.EntityID == -1)
				{
					return;
				}
				else
				{
					auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
					auto targetTransform = Gear::EntitySystem::GetTransform2D(worldData.EntityID);

					physics->SetFollowTarget(&targetTransform->GetPosition());
					physics->ActivateFollowTarget();
				}
			}
		}
	};

}