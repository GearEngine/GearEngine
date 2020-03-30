#pragma once
#include "WormEnum.h"

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

	class WormWorldEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			GR_TRACE("No.{0} Entity get World event", entityID);

			auto worldData = std::any_cast<WorldData>(data);

			if (worldData.DataType == WorldDataType::PrepareNextPhase)
			{
				if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
					return;

				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto prevState = FSM->GetCurrentState();
				FSM->SetCurrentState(WormState::OnTurnOver);

				auto status = Gear::EntitySystem::GetStatus(entityID);
				status->SetNeedHandleData(WormStatusHandleType::Display, false);

				if(prevState != WormState::OnWaiting)
					status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(-0.2f));
			}
		}
	};

}
