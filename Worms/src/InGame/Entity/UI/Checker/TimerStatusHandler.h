#include "wmpch.h"
#include "TimerFSMHandler.h"

namespace InGame {

	class TimerUpHandler : public Gear::Status::StatusHandler
	{
		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto positionY = transform->GetPosition().y;
			if (positionY < g_TimerUpPos.y)
			{
				physics->SetExternalVector(glm::vec2(0.0f, 0.5f));
			}
			else
			{
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				transform->SetPosition(g_TimerUpPos);
				data.Handled = true;
			}
		}
	};

	class TimerDownHandler : public Gear::Status::StatusHandler
	{
		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto physics = Gear::EntitySystem::GetPhysics2D(entityID);
			auto positionY = transform->GetPosition().y;
			if (positionY > g_TimerDownPos.y)
			{
				Font::PrintFont(glm::vec3(-0.894f, positionY, ZOrder::z_FlatFont), glm::vec3(0.009f * 4, 0.02f * 4, 1.0f), "0", FontType::RedNumber, 0.037f);
				physics->SetExternalVector(glm::vec2(0.0f, -0.7f));
			}
			else
			{
				physics->SetExternalVector(glm::vec2(0.0f, 0.0f));
				transform->SetPosition(g_TimerDownPos);
				data.Handled = true;
			}
		}
	};
}