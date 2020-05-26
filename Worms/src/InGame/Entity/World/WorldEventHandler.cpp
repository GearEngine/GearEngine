#include "wmpch.h"
#include "WorldEventHandler.h"

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	std::vector<int> WorldWormData::s_LivingWorms = std::vector<int>();
	std::vector<int> WorldWormData::s_ActiveWorms = std::vector<int>();
	std::queue<int> WorldWormData::s_WaitingDyeQue = std::queue<int>();

	void WorldEventHandler::Handle(std::any data, int entityID, bool & handled)
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

			ObjectLayer::s_TeamInfo[teamName].Surrendered = true;

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
					}
					Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
					worldTimer->SetTimer(3.0f);
					worldTimer->Start();
					onShotgun = false;
					inDamageWormFirst = true;
					handled = true;
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
					handled = true;
					return;
				}

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
					}
					Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
					worldTimer->SetTimer(3.0f);
					worldTimer->Start();
					onDonkey = false;
					inDamageWormFirst = true;
					handled = true;
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
					}
					Gear::EntitySystem::GetFSM(entityID)->SetCurrentState(WorldState::OnWaiting);
					worldTimer->SetTimer(3.0f);
					worldTimer->Start();
					onShotgun = false;
					inDamageWormFirst = true;
					handled = true;
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

}