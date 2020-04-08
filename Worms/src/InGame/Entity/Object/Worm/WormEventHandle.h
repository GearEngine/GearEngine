#pragma once
#include "WormEnum.h"
#include "InGame/Entity/Object/Item/ItemEnum.h"

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

		inline void ItemWithdraw(int entityID)
		{
			auto animator = Gear::EntitySystem::GetAnimator2D(entityID);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto item = std::any_cast<Item::Name>(status->GetStat(WormInfo::SelectedItem));
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (item == Item::Bazooka)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatBazukaWithdraw);
					return;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatBazukaWithdraw);
					return;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpBazukaWithdraw);
					return;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpBazukaWithdraw);
					return;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownBazukaWithdraw);
					return;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownBazukaWithdraw);
					return;
				}
			}
		}

		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto worldData = std::any_cast<WorldData>(data);

			if (worldData.DataType == WorldDataType::PrepareNextPhase)
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
				{
					handled = true;
					return;
				}
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto prevState = FSM->GetCurrentState();
				if (prevState == WormState::OnReadyItemUse)
				{
					ItemWithdraw(entityID);
					FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
					prevState = WormState::OnItemWithdraw;
					FSM->SetCurrentState(WormState::OnItemWithdraw);
					
				}

				if (prevState != WormState::OnBreath && prevState != WormState::OnWaiting)
				{
					handled = false;
					return;
				}
				if (prevState == WormState::OnWaiting)
				{
					status->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, true);
				}

				handled = true;
				handleAnimator(entityID, prevState);

				FSM->SetCurrentState(WormState::OnTurnOver);
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				timer->SetTimer(1.5f);
				timer->Start();

				status->SetNeedHandleData(WormStatusHandleType::Display, false);

				if (prevState != WormState::OnWaiting)
				{
					GR_TRACE("Push DisplayPosChage -0.2f at Worm Event Handler Event(PrepareNextPhase)");
					status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(-0.2f));
				}
			}
			if (worldData.DataType == WorldDataType::NewStart)
			{
				handled = true;
			}

		}
	};

}
