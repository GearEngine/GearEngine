#include "wmpch.h"
#include "World.h"

namespace InGame {

	float World::s_LimitTurnTime = 0;
	int World::s_LimitSuddenDeathTurn = 0;
	int World::s_CurrentTurn = 0;

	World::World(const InitiateData& initData)
	{
		s_LimitSuddenDeathTurn = initData.LimitSuddenDeathTurn;
		s_LimitTurnTime = initData.LimitTurnTime;
		s_CurrentTurn = 0;

		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM,	Gear::ComponentID::Status, Gear::ComponentID::Timer
		});

		//Set Component specific
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WorldState::OnStart, new WorldOnStartHandler },			{ WorldState::OnPrepareRun, new WorldOnPrepareRunHandler },
			{ WorldState::OnQuitWindow, new WorldOnQuitWindowHandler},	{ WorldState::OnRunning, new WorldOnRunningHandler},
		});

		Gear::EntitySystem::SetStatus(m_ID, {
			{ WorldInfo::CurrentWorm, std::string("") }, { WorldInfo::DyeInfo, std::stack<int>()},
			{ WorldInfo::CurrnetTeam, std::string("") }, { WorldInfo::CurrentTeamColor, TeamColor::Blue },
			{ WorldInfo::TeamInfo, initData.Teams },	 { WorldInfo::TeamInfoBlink, false }
		});

		Gear::EntitySystem::SetStatusHanlder(m_ID, {
			{ WorldStatusHandleType::DisplayWaitingCount, Gear::CreateRef<WorldDisplayWaitingCountHandler>() },
			{ WorldStatusHandleType::DisplayTeamInfo, Gear::CreateRef<WorldDisplayTeamInfoHandler>() }
		});

		auto status = Gear::EntitySystem::GetStatus(m_ID);
		status->PushNeedHandleData(WorldStatusHandleType::DisplayTeamInfo,
			Gear::Status::StatHandleData(WorldTeamInfoDenoteData(Gear::TextureStorage::GetTexture2D("WormNameBorder"))));

		//Subscpribe EventChannel
		//Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseClick);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::World, Gear::CreateRef<WorldEventHandler>());
	}

	World::~World()
	{
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::MouseClick);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void World::Update(Gear::Timestep ts)
	{
	}

}