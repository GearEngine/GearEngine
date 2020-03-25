#include "wmpch.h"
#include "World.h"

#include "WorldEventHandler.h"

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
			Gear::ComponentID::FSM
		});

		//Set Component specific
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WorldState::OnStart, new WorldOnStartHandler },			{ WorldState::OnPrepareRun, new WorldOnPrepareRunHandler },
			{ WorldState::OnQuitWindow, new WorldOnQuitWindowHandler},	{ WorldState::OnRunning, new WorldOnRunningHandler},
		});

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseClick);
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