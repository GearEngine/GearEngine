#pragma once

#include "WorldState.h"
#include "Common/EventChannel.h"

namespace InGame {

	struct WorldDenoteData
	{
		WorldDenoteData(const Gear::Ref<Gear::Texture2D>& waitingTimeBorder)
			: WaitingTimeBorder(waitingTimeBorder)
		{
			float width = waitingTimeBorder->GetWidth();
			float height = waitingTimeBorder->GetHeight();

			WaitingTimeBorderTranslate =
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.8f, ZOrder::z_FlatChecker))
				* glm::scale(glm::mat4(1.0f), glm::vec3(width / 1500.0f, height / 1500.0f, 1.0f));

			WaitingTimeFontPosition = glm::vec3(0.0f, 0.8f, ZOrder::z_FlatFont);
		}

		glm::mat4 WaitingTimeBorderTranslate;
		glm::vec3 WaitingTimeFontPosition;

		Gear::Ref<Gear::Texture2D> WaitingTimeBorder;
		Gear::Ref<Gear::Texture2D> TeamHpBarBorder;
		Gear::Ref<Gear::Texture2D> TeamNameBorder;
	};

	class WorldEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewFollow)
			{
				auto status = Gear::EntitySystem::GetStatus(entityID);
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				auto changedWormData = std::any_cast<std::pair<std::string, std::string>>(worldData.Data);

				status->SetStat(WorldInfo::CurrnetTeam, changedWormData.first);
				status->SetStat(WorldInfo::CurrentWorm, changedWormData.second);
				status->PushNeedHandleData(WorldStatusHandleType::DisplayWaitingCount, Gear::Status::StatHandleData(WorldDenoteData(Gear::TextureStorage::GetTexture2D("WaitingTimeBorder"))));
				timer->SetTimer(5.0f);
				timer->Start();
			}
		}
	};


}