#pragma once

namespace InGame {

	class CameraExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			handled = true;
		}
	};

	class CameraMouseMoveEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto mouseMoveData = std::any_cast<MouseMoveData>(data);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			physics->PauseFollowingTarget();
			handled = true;

			physics->SetExternalVector(glm::vec2(mouseMoveData.dx, mouseMoveData.dy));
		}
	};

	class CameraWorldEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewFollow)
			{
				auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
				if (worldData.EntityID == -1)
				{
					physics->InActivateFollowTarget();
				}
				else
				{
					auto targetTransform = Gear::EntitySystem::GetTransform2D(worldData.EntityID);

					physics->SetFollowTarget(&targetTransform->GetPosition());
					physics->ActivateFollowTarget();
				}
				handled = true;
			}
			handled = true;
		}
	};

}