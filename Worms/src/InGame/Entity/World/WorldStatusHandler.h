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

	class WorldDisplayTeamInfoHandler : public Gear::Status::StatusHandler
	{
		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			static const float blinkDelay = 0.5f;
			static float elasedTime = 0.0f;
			static bool onWhite = false;

			auto& teamDenoteData = std::any_cast<WorldTeamInfoDenoteData>(data.Data);
			auto& teamInfoData = std::any_cast<std::vector<TeamInfo>>(statlist[WorldInfo::TeamInfo]);
			auto& currentTeamName = std::any_cast<std::string>(statlist[WorldInfo::CurrnetTeam]);
			auto blink = std::any_cast<bool>(statlist[WorldInfo::TeamInfoBlink]);
			auto nameBorder = teamDenoteData.NameBorder;

			glm::mat4 teamInfoTranslate = teamDenoteData.TeamInfoBorderTranslate;
			glm::vec3 teamInfoNamePos = teamDenoteData.TeamNameFontPosition;
			glm::mat4 IconTranslate = teamDenoteData.IconTranslate;
			glm::vec3 hpPosition = teamDenoteData.HpBasisBarPosition;
			int offsetCount = 0;

			for (int i = 0; i < teamInfoData.size(); ++i)
			{
				int curTotalHp = teamInfoData[i].CurrentTotalWormHp;
				if (curTotalHp == 0)
				{
					offsetCount = 0;
					continue;
				}
				teamInfoTranslate[3].y += offsetCount * teamDenoteData.yOffset;
				IconTranslate[3].y += offsetCount * teamDenoteData.yOffset;
				teamInfoNamePos.y += offsetCount * teamDenoteData.yOffset;
				hpPosition.y += offsetCount * teamDenoteData.yOffset;
				
				offsetCount = 1;

				std::string teamName = teamInfoData[i].TeamName;
				TeamColor::Color teamColor = teamInfoData[i].TeamColor;

				//HpBar
			
				int totalHp = teamInfoData[i].TotalWormHp;
				float hpRatio = curTotalHp / (float)totalHp;
				hpPosition.x = teamDenoteData.HpBarBasisPositionX - 0.103f * (1.0f - hpRatio);

				glm::vec3 hpScale(teamDenoteData.HpBarWidthUnit * hpRatio + 0.001f, teamDenoteData.HpBarHeightUnit, 1.0f);
				glm::mat4 hpTranslate = glm::translate(glm::mat4(1.0f), hpPosition) * glm::scale(glm::mat4(1.0f), hpScale);

				Gear::Renderer2D::DrawFixedQuad(teamInfoTranslate, nameBorder);
				Gear::Renderer2D::DrawFixedQuad(IconTranslate, teamInfoData[i].TeamIcon);

				if (blink && teamInfoData[i].TeamName == currentTeamName)
				{
					elasedTime += Gear::EntitySystem::GetTimer(entityID)->GetTick();

					if (elasedTime > blinkDelay)
					{
						elasedTime = 0.0f;
						onWhite = !onWhite;
					}

					if (onWhite)
					{
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.04f, 1.0f), teamName, FontType::WhiteSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("WhiteHpBar"));
					}
					else
					{
						switch (teamColor)
						{
						case InGame::TeamColor::Red:
							Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.04f, 1.0f), teamName, FontType::RedSmall, 0.02f);
							Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("RedHpBar"));
							break;
						case InGame::TeamColor::Blue:
							Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.04f, 1.0f), teamName, FontType::BlueSmall, 0.02f);
							Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("BlueHpBar"));
							break;
						}
					}
				}
				else
				{
					switch (teamColor)
					{
					case InGame::TeamColor::Red:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.04f, 1.0f), teamName, FontType::RedSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("RedHpBar"));
						break;
					case InGame::TeamColor::Blue:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.04f, 1.0f), teamName, FontType::BlueSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("BlueHpBar"));
						break;
					}
				}
				
			}
		}
	};

}