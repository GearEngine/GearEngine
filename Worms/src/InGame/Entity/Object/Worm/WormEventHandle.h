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
			float additionalRadiusRatio = 1.5f;
			switch (explosion.Size)
			{
			case Explosion::Size::Size25:
				radius = 25.0f * additionalRadiusRatio / 37.0f;
				externalRatio = 7.5f;
				break;
			case Explosion::Size::Size50:
				radius = 50.0f * additionalRadiusRatio / 37.0f;
				externalRatio = 10.0f;
				break;
			case Explosion::Size::Size75:
				radius = 75.0f * additionalRadiusRatio / 37.0f;
				externalRatio = 12.5f;
				break;
			case Explosion::Size::Size100:
				radius = 100.0f * additionalRadiusRatio / 37.0f;
				externalRatio = 15.0f;
				break;
			}
			power = explosion.Power;
			Gear::EventSystem::DispatchEventOnce(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::DamageWorm)));

			if (distance < radius)
			{
				float damageRatio = 1.0f - distance / radius;

				glm::vec2 externalVector = glm::normalize(glm::vec2(dx, dy));
				externalVector = glm::vec2(externalVector.x * externalRatio * damageRatio, externalVector.y * externalRatio * damageRatio);

				auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto status = Gear::EntitySystem::GetStatus(entityID);
				auto& curExternalVector = physics->GetExternalVector();
				curExternalVector.x += externalVector.x;
				curExternalVector.y += externalVector.y;

				auto curDamage = std::any_cast<int>(status->GetStat(WormInfo::Damage));
				status->SetStat(WormInfo::Damage, int(damageRatio * power + curDamage));

				FSM->SetCurrentState(WormState::OnAttacked);
				GR_TRACE("{0} OnAttacked", Gear::EntitySystem::GetEntity(entityID)->GetName());

				physics->ActivateGravity();
				physics->ResetGravityAccelation();
				physics->SetPixelCollisionHandler("OnAir");
			}
			
			handled = true;
		}
	};

	class WormWorldEventHandler : public Gear::EventHandler
	{
		bool timerSetted = false;

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
			auto item = std::any_cast<ItemInfo::Number>(status->GetStat(WormInfo::SelectedItem));
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			if (item == ItemInfo::Number::Bazooka)
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
			if (item == ItemInfo::Number::Banana)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatBananaWithdraw);
					return;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatBananaWithdraw);
					return;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpBananaWithdraw);
					return;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpBananaWithdraw);
					return;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownBananaWithdraw);
					return;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownBananaWithdraw);
					return;
				}
			}
			if (item == ItemInfo::Number::Grenade)
			{
				switch (dir)
				{
				case InGame::WormInfo::LeftFlat:
					animator->PlayAnimation(WormState::OnLeftFlatGrenadeWithdraw);
					return;
				case InGame::WormInfo::RightFlat:
					animator->PlayAnimation(WormState::OnRightFlatGrenadeWithdraw);
					return;
				case InGame::WormInfo::LeftUp:
					animator->PlayAnimation(WormState::OnLeftUpGrenadeWithdraw);
					return;
				case InGame::WormInfo::RightUp:
					animator->PlayAnimation(WormState::OnRightUpGrenadeWithdraw);
					return;
				case InGame::WormInfo::LeftDown:
					animator->PlayAnimation(WormState::OnLeftDownGrenadeWithdraw);
					return;
				case InGame::WormInfo::RightDown:
					animator->PlayAnimation(WormState::OnRightDownGrenadeWithdraw);
					return;
				}
			}
		}

		virtual void Handle(std::any data, int entityID, bool& handled) override;
	};
}
