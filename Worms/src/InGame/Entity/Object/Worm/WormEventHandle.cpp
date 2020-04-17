#include "wmpch.h"
#include "WormEventHandle.h"

#include "InGame/Entity/World/WorldFSMHandler.h"

void InGame::WormWorldEventHandler::Handle(std::any data, int entityID, bool & handled)
{
	auto worldData = std::any_cast<WorldData>(data);

	if (worldData.DataType == WorldDataType::PrepareNextPhase)
	{
		GR_TRACE("Worm get PrepareNextPhase event");
		auto status = Gear::EntitySystem::GetStatus(entityID);
		if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
		{
			handled = true;
			return;
		}
		auto FSM = Gear::EntitySystem::GetFSM(entityID);
		auto prevState = FSM->GetCurrentState();
		if (prevState == WormState::OnReadyItemUse)
		{
			ItemWithdraw(entityID);
			FSM->GetHandler(WormState::OnReadyItemUse)->OnOut(entityID);
			prevState = WormState::OnItemWithdraw;
			FSM->SetCurrentState(WormState::OnItemWithdraw);
		}
		if (prevState != WormState::OnBreath && prevState != WormState::OnWaiting && prevState != WormState::OnNothing)
		{
			handled = false;
			return;
		}
		if (prevState == WormState::OnWaiting)
		{
			status->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, true);
		}

		auto timer = Gear::EntitySystem::GetTimer(entityID);
		if (!timerSetted)
		{
			FSM->SetCurrentState(WormState::OnTurnOver);
			timer->SetTimer(1.5f);
			timer->Start();
			handleAnimator(entityID, prevState);
			timerSetted = true;
		}

		if (prevState != WormState::OnWaiting)
		{
			status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 0.0f)));
		}

		if (timer->isExpired())
		{
			int damagedWormCount = 0;
			for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
			{
				if (WorldWormData::s_LivingWorms[i] == entityID)
				{
					continue;
				}
				auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
				if (curState == WormState::OnNothing || curState == WormState::OnUnderWater)
				{
					++damagedWormCount;
				}
			}

			if (!damagedWormCount)
			{
				handled = true;
				timerSetted = false;
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
			}
		}
	}
	if (worldData.DataType == WorldDataType::NewStart)
	{
		handled = true;
	}

}
