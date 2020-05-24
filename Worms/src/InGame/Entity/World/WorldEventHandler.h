#pragma once

#include "WorldState.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"
#include "Common/EventChannel.h"

namespace InGame {

	class WorldWormData
	{
	public: 
		static std::vector<int> s_LivingWorms;
		static std::vector<int> s_ActiveWorms;
		static std::queue<int> s_WaitingDyeQue;
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
		bool InFirst = true;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Timer> worldTimer;

		bool inDamageWormFirst = true;
		bool onShotgun = false;
		bool onDonkey = false;
		int curWorm;

		void init(int entityID)
		{
			status = Gear::EntitySystem::GetStatus(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);
			worldTimer = Gear::EntitySystem::GetTimer(entityID);

			InFirst = false;
		}

		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			if (InFirst)
			{
				init(entityID);
			}
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewFollow)
			{
				bool isWorm = false;
				for (int i = 0; i < WorldWormData::s_ActiveWorms.size(); ++i)
				{
					if (worldData.EntityID == WorldWormData::s_ActiveWorms[i])
					{
						isWorm = true;
						break;
					}
				}

				if (!isWorm)
				{
					handled = true;
					return;
				}

				auto worldState = FSM->GetCurrentState();
				if (worldState == WorldState::OnGameVictory)
				{
					handled = true;
					return;
				}

				auto changedWormData = std::any_cast<std::pair<std::string, std::string>>(worldData.Data);

				status->SetStat(WorldInfo::CurrnetTeam, changedWormData.first);
				status->SetStat(WorldInfo::CurrentWormName, changedWormData.second);
				status->SetStat(WorldInfo::CurrentWormID, worldData.EntityID);
				status->SetStat(WorldInfo::CurrentTeamColor, std::any_cast<TeamColor::Color>(Gear::EntitySystem::GetStatus(worldData.EntityID)->GetStat(WormInfo::TeamColor)));
				status->SetStat(WorldInfo::TeamInfoBlink, true);
				status->PushNeedHandleData(WorldStatusHandleType::DisplayWaitingCount, Gear::Status::StatHandleData(WorldDenoteData(Gear::TextureStorage::GetTexture2D("WaitingTimeBorder"))));
				worldTimer->SetTimer(5.0f);
				worldTimer->Start();
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::RunningStart)
			{
				status->SetNeedHandleData(WorldStatusHandleType::DisplayWaitingCount, false, true);
				status->GetStatusHandler(WorldStatusHandleType::DisplayWaitingCount)->OnOut();
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::PrepareNextPhase)
			{
				status->SetStat(WorldInfo::TeamInfoBlink, false);
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::TeamInfoBlinkOff)
			{
				status->SetStat(WorldInfo::TeamInfoBlink, false);
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::Surrender)
			{
				int surrenderWormID = worldData.EntityID;
				auto womrStatus = Gear::EntitySystem::GetStatus(surrenderWormID);
				auto teamName = std::any_cast<std::string>(womrStatus->GetStat(WormInfo::TeamName));

				for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
				{
					auto status = Gear::EntitySystem::GetStatus(WorldWormData::s_LivingWorms[i]);
					auto team = std::any_cast<std::string>(status->GetStat(WormInfo::TeamName));
					if (team == teamName)
					{
						if (surrenderWormID != WorldWormData::s_LivingWorms[i])
						{
							auto animator = Gear::EntitySystem::GetAnimator2D(WorldWormData::s_LivingWorms[i]);
							auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
							switch (dir)
							{
							case InGame::WormInfo::LeftFlat:
								animator->PlayAnimation(WormState::OnLeftFlatSurrenderOn);
								break;
							case InGame::WormInfo::RightFlat:
								animator->PlayAnimation(WormState::OnRightFlatSurrenderOn);
								break;
							case InGame::WormInfo::LeftUp:
								animator->PlayAnimation(WormState::OnLeftUpSurrenderOn);
								break;
							case InGame::WormInfo::RightUp:
								animator->PlayAnimation(WormState::OnRightUpSurrenderOn);
								break;
							case InGame::WormInfo::LeftDown:
								animator->PlayAnimation(WormState::OnLeftDownSurrenderOn);
								break;
							case InGame::WormInfo::RightDown:
								animator->PlayAnimation(WormState::OnRightDownSurrenderOn);
								break;
							}
							status->SetStat(WormInfo::Surrendered, true);
						}
						for (auto it = WorldWormData::s_ActiveWorms.begin(); it != WorldWormData::s_ActiveWorms.end(); ++it)
						{
							if (*it == WorldWormData::s_LivingWorms[i])
							{
								WorldWormData::s_ActiveWorms.erase(it);
								break;
							}
						}
					}
				}
				handled = true;
				return;
			}

			if (worldData.DataType == WorldDataType::DamageWorm)
			{
				if (inDamageWormFirst)
				{
					inDamageWormFirst = false;
					curWorm = std::any_cast<int>(status->GetStat(WorldInfo::CurrentWormID));
					if (std::any_cast<int>(Gear::EntitySystem::GetStatus(curWorm)->GetStat(WormInfo::ShotgunUseCnt)) == 1)
					{
						onShotgun = true;
					}
					if (std::any_cast<ItemInfo::Number>(Gear::EntitySystem::GetStatus(curWorm)->GetStat(WormInfo::SelectedItem)) == ItemInfo::Donkey)
					{
						onDonkey = true;
					}
				}

				std::vector<int> damagedWorm;
				if (FSM->GetCurrentState() == WorldState::OnWaiting)
				{
					FSM->SetCurrentState(WorldState::OnRunning);
				}

				if (onShotgun)
				{
					for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
					{
						auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
						if (curState != WormState::OnNothing && curState != WormState::OnNotMyTurn)
						{
							if (WorldWormData::s_LivingWorms[i] == curWorm)
							{
								continue;
							}
							else
							{
								return;
							}
						}
						if (curState == WormState::OnNothing)
						{
							damagedWorm.push_back(WorldWormData::s_LivingWorms[i]);
						}
					}
					if (damagedWorm.size())
					{
						for (int i = 0; i < damagedWorm.size(); ++i)
						{
							auto status = Gear::EntitySystem::GetStatus(damagedWorm[i]);
							auto timer = Gear::EntitySystem::GetTimer(damagedWorm[i]);
							timer->SetTimer(1.0f);
							timer->Start();
							GR_TRACE("dispatch damage {0}", Gear::EntitySystem::GetEntity(damagedWorm[i])->GetName());
							status->SetNeedHandleData(WormStatusHandleType::Damaged, false);
							Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
							worldTimer->SetTimer(3.0f);
							worldTimer->Start();
							onShotgun = false;
							inDamageWormFirst = true;
							handled = true;
						}
					}
					else
					{
						Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
						worldTimer->SetTimer(1.0f);
						worldTimer->Start();
						onShotgun = false;
						inDamageWormFirst = true;
						handled = true;
					}
				}
				else if (onDonkey)
				{
					int donkeyId = Gear::EntitySystem::GetEntityIDFromName("Donkey");
					if (Gear::EntitySystem::IsEntityActivated(donkeyId))
					{
						return;
					}

					for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
					{
						auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
						if (curState != WormState::OnNothing && curState != WormState::OnNotMyTurn)
						{
							if (WorldWormData::s_LivingWorms[i] == curWorm)
							{
								continue;
							}
							else
							{
								return;
							}
						}
						if (std::any_cast<int>(Gear::EntitySystem::GetStatus(WorldWormData::s_LivingWorms[i])->GetStat(WormInfo::Damage)));
						{
							damagedWorm.push_back(WorldWormData::s_LivingWorms[i]);
						}
					}
					if (damagedWorm.size())
					{
						for (int i = 0; i < damagedWorm.size(); ++i)
						{
							auto status = Gear::EntitySystem::GetStatus(damagedWorm[i]);
							auto timer = Gear::EntitySystem::GetTimer(damagedWorm[i]);
							timer->SetTimer(1.0f);
							timer->Start();
							GR_TRACE("dispatch damage {0}", Gear::EntitySystem::GetEntity(damagedWorm[i])->GetName());
							status->SetNeedHandleData(WormStatusHandleType::Damaged, false);
							Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
							worldTimer->SetTimer(3.0f);
							worldTimer->Start();
							onDonkey = false;
							inDamageWormFirst = true;
							handled = true;
						}
					}
					else
					{
						Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
						worldTimer->SetTimer(1.0f);
						worldTimer->Start();
						onDonkey = false;
						inDamageWormFirst = true;
						handled = true;
					}
				}
				else
				{
					for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
					{
						auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
						if (curState != WormState::OnNothing && curState != WormState::OnNotMyTurn)
						{
							return;
						}
						if (curState == WormState::OnNothing)
						{
							damagedWorm.push_back(WorldWormData::s_LivingWorms[i]);
						}
					}
					if (damagedWorm.size())
					{
						for (int i = 0; i < damagedWorm.size(); ++i)
						{
							auto status = Gear::EntitySystem::GetStatus(damagedWorm[i]);
							auto timer = Gear::EntitySystem::GetTimer(damagedWorm[i]);
							timer->SetTimer(1.0f);
							timer->Start();
							GR_TRACE("dispatch damage {0}", Gear::EntitySystem::GetEntity(damagedWorm[i])->GetName());
							status->SetNeedHandleData(WormStatusHandleType::Damaged, false);
							Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
							worldTimer->SetTimer(3.0f);
							worldTimer->Start();
							onShotgun = false;
							inDamageWormFirst = true;
							handled = true;
						}
					}
					else
					{
						Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
						worldTimer->SetTimer(1.0f);
						worldTimer->Start();
						onShotgun = false;
						inDamageWormFirst = true;
						handled = true;
					}
				}
				return;
			}
			if (worldData.DataType == WorldDataType::NewStart)
			{
				auto prevState = FSM->GetCurrentState();

				if (prevState != WorldState::OnGameDraw && prevState != WorldState::OnGameVictory)
				{
					FSM->SetCurrentState(WorldState::OnPrepareRun);
				}
				handled = true;
				return;
			}
			if (worldData.DataType == WorldDataType::CreatedWorm)
			{
				WorldWormData::s_LivingWorms.push_back(worldData.EntityID);
				WorldWormData::s_ActiveWorms.push_back(worldData.EntityID);
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
			if (worldData.DataType == WorldDataType::WormDie)
			{
				for (auto worm = WorldWormData::s_LivingWorms.begin(); worm != WorldWormData::s_LivingWorms.end(); ++worm)
				{
					if (*worm == worldData.EntityID)
					{
						WorldWormData::s_WaitingDyeQue.push(*worm);
						for (auto activeWorm = WorldWormData::s_ActiveWorms.begin(); activeWorm != WorldWormData::s_ActiveWorms.end(); ++activeWorm)
						{
							if (*worm == *activeWorm)
							{
								WorldWormData::s_ActiveWorms.erase(activeWorm);
								break;
							}
						}
						WorldWormData::s_LivingWorms.erase(worm);
						handled = true;
						return;
					}
				}
			}

			handled = true;
		}
	};
	
	
}