#pragma once

#include "WorldState.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"
#include "Common/EventChannel.h"

namespace InGame {

	class WorldWormData
	{
	public: 
		static std::vector<int> s_LivingWorms;
	};

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

			WaitingTimeFontPosition = glm::vec3(0.05f, 0.8f, ZOrder::z_FlatFont);
		}

		glm::mat4 WaitingTimeBorderTranslate;
		glm::vec3 WaitingTimeFontPosition;

		Gear::Ref<Gear::Texture2D> WaitingTimeBorder;
	};

	struct WorldTeamInfoDenoteData
	{
		WorldTeamInfoDenoteData() = default;
		WorldTeamInfoDenoteData(const Gear::Ref<Gear::Texture2D>& nameBorder)
			: NameBorder(nameBorder)
		{
			float width = nameBorder->GetWidth();
			float height = NameBorder->GetHeight();

			TeamNameBorderWidthUnit = width / 1700.0f;
			TeamNameBorderHeightUnit = height / 1200.0f;

			TeamInfoBorderTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.092f, -0.94f, ZOrder::z_FlatChecker)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(TeamNameBorderWidthUnit, TeamNameBorderHeightUnit, 1.0f));

			TeamNameFontPosition = glm::vec3(-0.082f, -0.94f, ZOrder::z_FlatFont);
			yOffset = 0.06f;

			float iconWidth = 15;
			float iconHeight = 15;
			IconTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.94, ZOrder::z_FlatCheckerImg))
				* glm::scale(glm::mat4(1.0f), glm::vec3(iconWidth / 380.0f, iconHeight / 270.0f, 1.0f));

			HpBarBasisPositionX = 0.128f;
			HpBasisBarPosition = glm::vec3(HpBarBasisPositionX, -0.94f, ZOrder::z_FlatCheckerImg);

			HpBarWidthUnit = 104.0f / 500.0f;
			HpBarHeightUnit = 17.0f / 300.0f;

		}

		glm::mat4 TeamInfoBorderTranslate;
		glm::mat4 IconTranslate;
		glm::vec3 TeamNameFontPosition;
		glm::vec3 HpBasisBarPosition;

		float TeamNameBorderWidthUnit;
		float TeamNameBorderHeightUnit;

		float HpBarWidthUnit;
		float HpBarHeightUnit;
		float HpBarBasisPositionX;

		float yOffset;

		Gear::Ref<Gear::Texture2D> NameBorder;

	};

	class WorldEventHandler : public Gear::EventHandler
	{
		
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			auto status = Gear::EntitySystem::GetStatus(entityID);
			if (worldData.DataType == WorldDataType::NewFollow)
			{
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				auto timer = Gear::EntitySystem::GetTimer(entityID);
				auto changedWormData = std::any_cast<std::pair<std::string, std::string>>(worldData.Data);

				status->SetStat(WorldInfo::CurrnetTeam, changedWormData.first);
				status->SetStat(WorldInfo::CurrentWorm, changedWormData.second);
				status->SetStat(WorldInfo::TeamInfoBlink, true);
				status->PushNeedHandleData(WorldStatusHandleType::DisplayWaitingCount, Gear::Status::StatHandleData(WorldDenoteData(Gear::TextureStorage::GetTexture2D("WaitingTimeBorder"))));
				timer->SetTimer(5.0f);
				timer->Start();
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::RunningStart)
			{
				status->SetNeedHandleData(WorldStatusHandleType::DisplayWaitingCount, false, true);
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::PrepareNextPhase)
			{
				GR_TRACE("World Reseive PrepareNext");

				status->SetStat(WorldInfo::TeamInfoBlink, false);
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::DamageWorm)
			{
				GR_TRACE("World Reseive DamageWorm");

				std::vector<int> damagedWorm;
				for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
				{
					status->SetStat(WorldInfo::TeamInfoBlink, false);
					auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
					if (curState != WormState::OnNothing && curState != WormState::OnNotMyTurn)
					{
						return;
					}
					if(curState == WormState::OnNothing)
					{
						damagedWorm.push_back(WorldWormData::s_LivingWorms[i]);
					}
				}
				for (int i = 0; i < damagedWorm.size(); ++i)
				{
					auto status = Gear::EntitySystem::GetStatus(damagedWorm[i]);
					auto timer = Gear::EntitySystem::GetTimer(damagedWorm[i]);
					timer->SetTimer(1.0f);
					timer->Start();
					status->SetNeedHandleData(WormStatusHandleType::Damaged, false);
				}
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::CreatedWorm)
			{
				WorldWormData::s_LivingWorms.push_back(worldData.EntityID);
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::GetDamageWorm)
			{
				auto& teamInfoData = std::any_cast<std::vector<TeamInfo>>(WorldInfo::TeamInfo);
				auto& damageData = std::any_cast<DamageData>(data);
				for (auto team : teamInfoData)
				{
					if (team.TeamName == damageData.WormTeamName)
					{
						team.CurrentTotalWormHp -= damageData.Damage;
					}
				}
				handled = true;
				return;
			}

			handled = true;
		}
	};
	

}