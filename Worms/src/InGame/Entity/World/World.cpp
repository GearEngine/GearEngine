#include "wmpch.h"
#include "World.h"

#include "WorldEventHandler.h"
#include "WorldFSMHandler.h"

namespace InGame {

	World::World()
	{
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
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void World::Update(Gear::Timestep ts)
	{
	}

}