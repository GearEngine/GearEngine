#pragma once

#include "WorldFSMHandler.h"

namespace InGame {

	class WorldDisplayWaitingCountHandler : public Gear::Status::StatusHandler
	{
		bool twoBeep = false;
		bool oneBeep = false;
		bool zeroBeep = false;

		virtual void OnOut() override
		{
			twoBeep = false;
			oneBeep = false;
			zeroBeep = false;
		}

		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto printData = std::any_cast<WorldDenoteData>(data.Data);
			static const float blinkDelay = 0.5f;
			static float elapsedTime = 0.0f;
			static bool onWhite = false;

			std::string outputStr = std::any_cast<std::string>(statlist[WorldInfo::CurrnetTeam]);
			float remainTime = Gear::EntitySystem::GetTimer(entityID)->GetRemainTime();
			outputStr += " : " + std::to_string((int)remainTime + 1) + " Sec";
			int length = outputStr.length();
			TeamColor::Color teamColor = std::any_cast<TeamColor::Color>(statlist[WorldInfo::CurrentTeamColor]);

			Gear::Renderer2D::DrawFixedQuad(printData.WaitingTimeBorderTranslate, printData.WaitingTimeBorder);

			if (remainTime <= 2.0f)
			{
				if (!twoBeep)
				{
					twoBeep = true;
					PLAY_SOUND_NAME("TIMERTICK", WormsSound::effect);
				}
			}
			if (remainTime <= 1.0f)
			{
				if (!oneBeep)
				{
					oneBeep = true;
					PLAY_SOUND_NAME("TIMERTICK", WormsSound::effect);
				}
			}
			if (remainTime <= 0.0f)
			{
				if (!zeroBeep)
				{
					zeroBeep = true;
					PLAY_SOUND_NAME("TIMERTICK", WormsSound::effect);
				}
			}

			elapsedTime += Gear::EntitySystem::GetTimer(entityID)->GetTick();
			if (elapsedTime > blinkDelay)
			{
				onWhite = !onWhite;
				elapsedTime = 0.0f;
			}

			if (onWhite)
			{
				Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::WhiteSmall, 0.03f);
			}
			else 
			{
				switch (teamColor)
				{
				case InGame::TeamColor::Red:
					Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::RedSmall, 0.03f);
					break;
				case InGame::TeamColor::Blue:
					Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::BlueSmall, 0.03f);
					break;
				case InGame::TeamColor::Green:
					Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::GreenSmall, 0.03f);
					break;
				case InGame::TeamColor::Yellow:
					Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::YellowSmall, 0.03f);
					break;
				case InGame::TeamColor::Purple:
					Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::PurpleSmall, 0.03f);
					break;
				case InGame::TeamColor::Sky:
					Font::PrintFont(printData.WaitingTimeFontPosition, glm::vec3(0.05f, 0.07f, 1.0f), outputStr, FontType::SkySmall, 0.03f);
					break;
				}
			}
			
			if (remainTime == 0.0f)
			{
				onWhite = false;
				OnOut();
				data.Handled = true;
			}
		}
	};

	class WorldDisplayTeamInfoHandler : public Gear::Status::StatusHandler
	{
		const float blinkDelay = 0.5f;
		float elasedTime = 0.0f;
		bool onWhite = false;
		bool inFirst = true;
		WorldTeamInfoDenoteData teamDenoteData;
		std::string currentTeamName;
		bool blink;
		Gear::Ref<Gear::Texture2D> nameBorder;

		glm::mat4 teamInfoTranslate;
		glm::vec3 teamInfoNamePos;
		glm::mat4 IconTranslate;
		glm::vec3 hpPosition;

		glm::mat4 originTeamInfoTranslate;
		glm::vec3 originTeamInfoNamePos;
		glm::mat4 originIconTranslate;
		glm::vec3 originHpPosition;

		std::vector<TeamInfo> teamInfoData;

	private:
		inline void init(Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist)
		{
			teamDenoteData = std::any_cast<WorldTeamInfoDenoteData>(data.Data);
			nameBorder = teamDenoteData.NameBorder;

			originTeamInfoTranslate = teamDenoteData.TeamInfoBorderTranslate;
			originTeamInfoNamePos = teamDenoteData.TeamNameFontPosition;
			originIconTranslate = teamDenoteData.IconTranslate;
			originHpPosition = teamDenoteData.HpBasisBarPosition;
		}

		inline void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
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
			}
			
		
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
	};

	class WorldDisplayMasageHandler : public Gear::Status::StatusHandler
	{

		bool inFirst = true;
		bool OnAwake = true;
		float pastTime = 0.0f;
		float duration = 2.0f;

		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::Texture2D> massageBorder;

		glm::mat4 m_Translate;
		glm::vec3 m_FontScale;
		glm::vec3 m_FontPosition;
		
		WorldMassage massage;
		std::string massageStr;
		FontType::Type fontType;

		void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			massageBorder = Gear::TextureStorage::GetTexture2D("WorldMassageBorder");

			m_Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.88f, ZOrder::z_FlatChecker)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.07f, 1.0f));
			m_FontPosition = glm::vec3(0.05f, 0.88f, ZOrder::z_FlatFont);
			m_FontScale = glm::vec3(0.04f, 0.06f, 1.0f);
		}

		void init(int entityID)
		{
			pastTime = 0.0f;
			inFirst = false;
		}

		void OnOut() override
		{
			inFirst = true;
		}

		void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}
			if (inFirst)
			{
				init(entityID);
				massage = std::any_cast<WorldMassage>(data.Data);
				massageStr = massage.Str;
				fontType = massage.Type;
			}

			pastTime += timer->GetTick();

			Gear::Renderer2D::DrawFixedQuad(m_Translate, massageBorder);
			Font::PrintFont(m_FontPosition, m_FontScale, massageStr, fontType, 0.02f);

			if (pastTime > duration)
			{
				data.Handled = true;
				OnOut();
				return;
			}
		}
	};
}