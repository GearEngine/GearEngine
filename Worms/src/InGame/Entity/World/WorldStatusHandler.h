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

		void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override;
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