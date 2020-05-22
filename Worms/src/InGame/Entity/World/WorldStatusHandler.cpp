#include "wmpch.h"
#include "WorldStatusHandler.h"

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	void WorldDisplayTeamInfoHandler::Handle(int entityID, Gear::Status::StatHandleData & data, std::unordered_map<Gear::EnumType, std::any>& statlist)
	{
		if (inFirst)
		{
			inFirst = false;
			init(data, statlist);
		}
		blink = std::any_cast<bool>(statlist[WorldInfo::TeamInfoBlink]);
		currentTeamName = std::any_cast<std::string>(statlist[WorldInfo::CurrnetTeam]);
		teamInfoData = std::any_cast<std::vector<TeamInfo>>(statlist[WorldInfo::TeamInfo]);

		int offsetCount = 0;

		teamInfoTranslate = originTeamInfoTranslate;
		teamInfoNamePos = originTeamInfoNamePos;
		IconTranslate = originIconTranslate;
		hpPosition = originHpPosition;

		for (int i = 0; i < teamInfoData.size(); ++i)
		{
			teamInfoData[i].CurrentTotalWormHp = 0;
			for (int j = 0; j < WorldWormData::s_LivingWorms.size(); ++j)
			{

				auto status = Gear::EntitySystem::GetStatus(WorldWormData::s_LivingWorms[j]);
				if (teamInfoData[i].TeamName == std::any_cast<std::string>(status->GetStat(WormInfo::TeamName)))
				{
					teamInfoData[i].CurrentTotalWormHp += std::any_cast<int>(status->GetStat(WormInfo::Hp));
				}
			}
			if (ObjectLayer::s_TeamInfo[teamInfoData[i].TeamName].TotalWormHp != teamInfoData[i].CurrentTotalWormHp)
			{
				ObjectLayer::s_TeamInfo[teamInfoData[i].TeamName].TotalWormHp = teamInfoData[i].CurrentTotalWormHp;
			}
		}


		for (int i = 0; i < teamInfoData.size(); ++i)
		{
			int curTotalHp = teamInfoData[i].CurrentTotalWormHp;
			if (curTotalHp == 0)
			{
				continue;
			}
			

			teamInfoTranslate[3].y = originTeamInfoTranslate[3].y + offsetCount * teamDenoteData.yOffset;
			IconTranslate[3].y = originIconTranslate[3].y + offsetCount * teamDenoteData.yOffset;
			teamInfoNamePos.y = originTeamInfoNamePos.y + offsetCount * teamDenoteData.yOffset;
			hpPosition.y = originHpPosition.y + offsetCount * teamDenoteData.yOffset;

			++offsetCount;

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
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::WhiteSmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("WhiteHpBar"));
				}
				else
				{
					switch (teamColor)
					{
					case InGame::TeamColor::Red:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::RedSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("RedHpBar"));
						break;
					case InGame::TeamColor::Blue:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::BlueSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("BlueHpBar"));
						break;
					case InGame::TeamColor::Green:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::GreenSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("GreenHpBar"));
						break;
					case InGame::TeamColor::Yellow:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::YellowSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("YellowHpBar"));
						break;
					case InGame::TeamColor::Purple:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::PurpleSmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("PurpleHpBar"));
						break;
					case InGame::TeamColor::Sky:
						Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::SkySmall, 0.02f);
						Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("SkyHpBar"));
						break;
					}
				}
			}
			else
			{
				switch (teamColor)
				{
				case InGame::TeamColor::Red:
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::RedSmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("RedHpBar"));
					break;
				case InGame::TeamColor::Blue:
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::BlueSmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("BlueHpBar"));
					break;
				case InGame::TeamColor::Green:
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::GreenSmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("GreenHpBar"));
					break;
				case InGame::TeamColor::Yellow:
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::YellowSmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("YellowHpBar"));
					break;
				case InGame::TeamColor::Purple:
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::PurpleSmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("PurpleHpBar"));
					break;
				case InGame::TeamColor::Sky:
					Font::PrintFont(teamInfoNamePos, glm::vec3(0.04f, 0.05f, 1.0f), teamName, FontType::SkySmall, 0.02f);
					Gear::Renderer2D::DrawFixedQuad(hpTranslate, Gear::TextureStorage::GetTexture2D("SkyHpBar"));
					break;
				}
			}

		}
	}

}
