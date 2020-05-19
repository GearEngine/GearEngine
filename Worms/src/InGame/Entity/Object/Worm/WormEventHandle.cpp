#include "wmpch.h"
#include "WormEventHandle.h"

#include "InGame/Entity/World/WorldFSMHandler.h"

void InGame::WormWorldEventHandler::Handle(std::any data, int entityID, bool & handled)
{
	auto worldData = std::any_cast<WorldData>(data);

	if (worldData.DataType == WorldDataType::PrepareNextPhase)
	{
		auto status = Gear::EntitySystem::GetStatus(entityID);
		if (GameMode::Bit::ModeBit == GameMode::NetWork)
		{
			if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::NetController))
			{
				handled = true;
				return;
			}
		}
		else
		{
			if (!Gear::EntitySystem::IsComponenetActivate(entityID, Gear::ComponentID::Controller))
			{
				handled = true;
				return;
			}
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
		if (prevState != WormState::OnBreath && prevState != WormState::OnWaiting && prevState != WormState::OnNotMyTurn && prevState != WormState::OnNothing)
		{
			handled = false;
			return;
		}
		

		auto timer = Gear::EntitySystem::GetTimer(entityID);
		FSM->SetCurrentState(WormState::OnTurnOver);
		timer->SetTimer(1.5f);
		timer->Start();
		handleAnimator(entityID, prevState);

		if (prevState == WormState::OnWaiting)
		{
			status->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, true);
			status->SetNeedHandleData(WormStatusHandleType::Display, false);
		}
		else
		{
			status->PushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 0.0f)));
		}
		handled = true;
		return;
	}
	handled = true;
}
