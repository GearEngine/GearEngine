#pragma once
#include "WormEnum.h"
#include "InGame/Entity/Object/Item/ItemEnum.h"

namespace InGame {

	class WormExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto explosion = std::any_cast<ExplosionData>(data);

			auto transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto wormPosition = transform->GetPosition();

			float dx = wormPosition.x - explosion.Position.x;
			float dy = wormPosition.y - explosion.Position.y;
			float distance = std::powf((dx), 2) + std::powf((dy), 2);
			distance = sqrt(distance);

			float radius, power, externalRatio;
			switch (explosion.Size)
			{
			case Explosion::Size::Size25:
				radius = 25.0f * 1.3f / 37.0f;
				power = 25.0f;
				externalRatio = 5.0f;
				break;
			case Explosion::Size::Size50:
				radius = 50.0f * 1.3f / 37.0f;
				power = 50.0f;
				externalRatio = 10.0f;
				break;
			case Explosion::Size::Size75:
				radius = 75.0f * 1.3f / 37.0f;
				power = 75.0f;
				externalRatio = 15.0f;
				break;
			case Explosion::Size::Size100:
				radius = 100.0f * 1.3f / 37.0f;
				power = 100.0f;
				externalRatio = 20.0f;
				break;
			}

			Gear::EventSystem::DispatchEventOnce(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::DamageWorm)));

			if (distance < radius)
			{
				float damageRatio = 1.0f - distance / radius;

				glm::vec2 externalVector = glm::normalize(glm::vec2(dx, dy));

				auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto status = Gear::EntitySystem::GetStatus(entityID);

				status->SetStat(WormInfo::Damage, int(damageRatio * power));

				FSM->SetCurrentState(WormState::OnAttacked);
				GR_TRACE("{0} OnAttacked", Gear::EntitySystem::GetEntity(entityID)->GetName());

				physics->ActivateGravity();
				physics->SetPixelCollisionHandler("OnAir");
				physics->SetExternalVector(glm::vec2(externalVector.x * externalRatio * damageRatio, externalVector.y * externalRatio * damageRatio));
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

				if (prevState != WormState::OnWaiting)
				{
					status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair( -0.2f, 0.0f )));
				}
			}
			if (worldData.DataType == WorldDataType::NewStart)
			{
				handled = true;
			}

		}
	};
}
