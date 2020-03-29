#pragma once

#include "WorldFSMHandler.h"

namespace InGame {

	class WorldDisplayWaitingCountHandler : public Gear::Status::StatusHandler
	{
		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto printData = std::any_cast<WorldDenoteData>(data.Data);

			std::string outputStr = std::any_cast<std::string>(statlist[WorldInfo::CurrnetTeam]);
			float remainTime = Gear::EntitySystem::GetTimer(entityID)->GetRemainTime();
			outputStr += " : " + std::to_string((int)remainTime + 1) + " Sec";
			int length = outputStr.length();
			TeamColor::Color teamColor = std::any_cast<TeamColor::Color>(statlist[WorldInfo::CurrentTeamColor]);

			Gear::Renderer2D::DrawFixedQuad(printData.WaitingTimeBorderTranslate, printData.WaitingTimeBorder);

			switch (teamColor)
			{
			case InGame::TeamColor::Red:
				Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.05f, 1.0f), outputStr, FontType::RedSmall, 0.2f);
				break;
			case InGame::TeamColor::Blue:
				Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.05f, 1.0f), outputStr, FontType::BlueSmall, 0.02f);
				break;
			}

			if (remainTime == 0.0f)
			{
				data.Handled = true;
			}
		}
	};

	class WorldDisplayTotalHpBarHandler : public Gear::Status::StatusHandler
	{
		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{



		}

	};

}