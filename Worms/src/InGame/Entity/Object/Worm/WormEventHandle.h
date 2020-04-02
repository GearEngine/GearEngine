#pragma once
#include "WormEnum.h"

namespace InGame {

	class WormExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
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
			handled = true;
		}
	};

	class WormWorldEventHandler : public Gear::EventHandler
	{
		inline void handleAnimator(int entityID, unsigned int prevState)
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			switch (dir)
			{
			case InGame::WormInfo::LeftFlat:
				animator->PlayAnimation(WormState::OnLeftFlatBreath);
				break;
			case InGame::WormInfo::RightFlat:
				animator->PlayAnimation(WormState::OnRightFlatBreath);
				break;
			case InGame::WormInfo::LeftUp:
				animator->PlayAnimation(WormState::OnLeftUpBreath);
				break;
			case InGame::WormInfo::RightUp:
				animator->PlayAnimation(WormState::OnRightUpBreath);
				break;
			case InGame::WormInfo::LeftDown:
				animator->PlayAnimation(WormState::OnLeftDownBreath);
				break;
			case InGame::WormInfo::RightDown:
				animator->PlayAnimation(WormState::OnRightDownBreath);
				break;
			}
		}

		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto worldData = std::any_cast<WorldData>(data);

			if (worldData.DataType == WorldDataType::PrepareNextPhase)
			{
				if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
				{
					handled = true;
					return;
				}
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto prevState = FSM->GetCurrentState();
				if (prevState != WormState::OnBreath)
				{
					handled = false;
					return;
				}

				handled = true;
				handleAnimator(entityID, prevState);

				FSM->SetCurrentState(WormState::OnTurnOver);
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(1.5f);
				timer->Start();

				auto status = Gear::EntitySystem::GetStatus(entityID);
				status->SetNeedHandleData(WormStatusHandleType::Display, false);

				if(prevState != WormState::OnWaiting)
					status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(-0.2f));
			}
			if (worldData.DataType == WorldDataType::NewStart)
			{
				handled = true;
			}

		}
	};

}
