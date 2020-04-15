#pragma once

namespace InGame {

	class GraveExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto explosion = std::any_cast<ExplosionData>(data);

			auto transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto gravePosition = transform->GetPosition();

			float dx = gravePosition.x - explosion.Position.x;
			float dy = gravePosition.y - explosion.Position.y;
			float distance = std::powf((dx), 2) + std::powf((dy), 2);
			distance = sqrt(distance);

			float radius, power, externalRatio;
			float additionalRadiusRatio = 1.5f;
			switch (explosion.Size)
			{
			case Explosion::Size::Size25:
				radius = 25.0f * additionalRadiusRatio / 37.0f;
				power = 25.0f;
				externalRatio = 7.5f;
				break;
			case Explosion::Size::Size50:
				radius = 50.0f * additionalRadiusRatio / 37.0f;
				power = 50.0f;
				externalRatio = 10.0f;
				break;
			case Explosion::Size::Size75:
				radius = 75.0f * additionalRadiusRatio / 37.0f;
				power = 75.0f;
				externalRatio = 12.5f;
				break;
			case Explosion::Size::Size100:
				radius = 100.0f * additionalRadiusRatio / 37.0f;
				power = 100.0f;
				externalRatio = 15.0f;
				break;
			}

			if (distance < radius)
			{
				auto physics = Gear::EntitySystem::GetPhysics2D(entityID);

				float damageRatio = 1.0f - distance / radius;
				glm::vec2 externalVector = glm::normalize(glm::vec2(dx, dy));

				if (dy > 0.0f)
				{
					physics->SetExternalVectorY(externalVector.y * externalRatio * damageRatio);
					physics->ActivateGravity();
					physics->ResetGravityAccelation();
				}
			}

			handled = true;
		}
	};
	

}