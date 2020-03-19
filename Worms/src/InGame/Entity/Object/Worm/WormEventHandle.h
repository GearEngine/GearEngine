#pragma once
#include "InGame/Data/EventData.h"

namespace InGame {

	class WormExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			GR_TRACE("No.{0} Entity get explosion event", entityID);

			auto explosion = std::any_cast<ExplosionData>(data);

			auto transformComponent = Gear::EntitySystem::GetTransform2D(entityID);
			auto entityPosition = transformComponent->GetPosition();

			float distance = std::powf((explosion.Position.x - entityPosition.x), 2) + std::powf((explosion.Position.y - entityPosition.y), 2);

			if (distance < powf(explosion.radius, 2))
			{
				GR_TRACE("No.{0} Entity Damaged by explosion", entityID);
			}
		}
	};

}
