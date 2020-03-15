#include "grpch.h"
#include "EntitySystem.h"

namespace Gear {

	int EntitySystem::s_EntityID = 0;

	std::queue<int> EntitySystem::m_SpareIDqueue = std::queue<int>();

	std::unordered_map<int, Ref<Entity>> EntitySystem::m_EntityPool = std::unordered_map<int, Ref<Entity>>();
	std::unordered_map<int, Ref<Entity>> EntitySystem::m_ActivateEntitys = std::unordered_map<int, Ref<Entity>>();

	std::vector<Ref<Animator2D>>	EntitySystem::m_Animators = std::vector<Ref<Animator2D>>();
	std::vector<Ref<Controller>>	EntitySystem::m_Controllers = std::vector<Ref<Controller>>();
	std::vector<Ref<FSM>>			EntitySystem::m_FSMs = std::vector<Ref<FSM>>();
	std::vector<Ref<SoundPlayer>>	EntitySystem::m_SoundPlayers = std::vector<Ref<SoundPlayer>>();
	std::vector<Ref<Transform2D>>	EntitySystem::m_Transforms = std::vector<Ref<Transform2D>>();
	std::vector<Ref<Drawer2D>>		EntitySystem::m_Drawer = std::vector<Ref<Drawer2D>>();
	std::vector<Ref<Physics>>		EntitySystem::m_Phisics = std::vector<Ref<Physics>>();


	void EntitySystem::Init()
	{
		m_Animators.resize(10000);
		m_Controllers.resize(10000);
		m_Drawer.resize(10000);
		m_FSMs.resize(10000);
		m_Phisics.resize(10000);
		m_SoundPlayers.resize(10000);
		m_Transforms.resize(10000);
	}

	void EntitySystem::ShutDown()
	{
		m_EntityPool.clear();
		m_ActivateEntitys.clear();

		m_Animators.clear();
		m_Controllers.clear();
		m_Drawer.clear();
		m_FSMs.clear();
		m_Phisics.clear();
		m_SoundPlayers.clear();
		m_Transforms.clear();
	}

	void EntitySystem::Update(Timestep ts)
	{
		GR_PROFILE_FUNCTION();

		for (auto& entity : m_ActivateEntitys)
		{
			int id = entity.first;
	
			if (m_Drawer[id])
			{
				m_Drawer[id]->Render();
			}
		}
	}

	void EntitySystem::Render()
	{

	}

	int EntitySystem::CreateEntity(bool activate)
	{
		int entityID;
		Ref<Entity> newEntity;

		if (m_SpareIDqueue.empty())
		{
		 	entityID = s_EntityID++;
			
			newEntity.reset(new Entity(entityID));

			m_EntityPool.insert({ entityID, newEntity });
			if (activate)
			{
				ActivateEntity(entityID);
			}
			GR_CORE_TRACE("{0} entity Created!", entityID);
		}
		else
		{
			entityID = m_SpareIDqueue.front();
			m_SpareIDqueue.pop();

			newEntity.reset(new Entity(entityID));

			m_EntityPool.insert({ entityID, newEntity });
			if (activate)
			{
				ActivateEntity(entityID);
			}
			GR_CORE_TRACE("{0} entity Created!", entityID);
		}
		return entityID;
	}

	void EntitySystem::ActivateEntity(int entityID)
	{
		auto entityInActivate = m_ActivateEntitys.find(entityID);
		if (entityInActivate == m_ActivateEntitys.end())
		{
			auto entityInPool = m_EntityPool.find(entityID);

			if (entityInPool != m_EntityPool.end())
			{
				entityInPool->second->m_OnActivate = true;
				GR_CORE_TRACE("{0} entity activated!", entityID);
				m_ActivateEntitys.insert({ entityID, entityInPool->second });
			}
			else
			{
				GR_CORE_WARN("{0} entity doen't exist!", entityID);
			}
		}
		else
		{
			GR_CORE_WARN("{0} entity aleady activated!", entityID);
		}
	}

	void EntitySystem::InActivateEntity(int entityID)
	{
		auto entityInActivate = m_ActivateEntitys.find(entityID);
		if (entityInActivate != m_ActivateEntitys.end())
		{
			entityInActivate->second->m_OnActivate = false;
			m_ActivateEntitys.erase(entityInActivate);
			GR_CORE_TRACE("{0} entity inactivated!", entityID);
		}
		else
		{
			GR_CORE_WARN("{0} entity aleady inactivated!", entityID);
		}
	}

	void EntitySystem::DeleteEntity(int entityID)
	{
		auto entityInPool = m_EntityPool.find(entityID);

		if (entityInPool != m_EntityPool.end())
		{
			bool onActivate = entityInPool->second->m_OnActivate;
			m_EntityPool.erase(entityInPool);
			if (onActivate)
			{
				auto entityInActivate = m_ActivateEntitys.find(entityID);
				m_ActivateEntitys.erase(entityInActivate);
			}
			GR_CORE_TRACE("{0} entity deleted!", entityID);
			m_Animators[entityID].reset();
			m_Controllers[entityID].reset();
			m_Drawer[entityID].reset();
			m_FSMs[entityID].reset();
			m_Phisics[entityID].reset();
			m_SoundPlayers[entityID].reset();
			m_Transforms[entityID].reset();
		}
		else
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
		}
	}
	


	void EntitySystem::AttachComponent(int entityID, const std::vector<ComponentID::ID>& components)
	{
		for (auto& id : components)
		{
			switch (id)
			{
			case ComponentID::ID::Animantor:   
				if (!m_Animators[entityID]) m_Animators[entityID].reset(new Animator2D());
				break;
			case ComponentID::ID::Controller:	
				if (!m_Controllers[entityID]) m_Controllers[entityID].reset(new Controller());
				break;
			case ComponentID::ID::Drawer:		
				if (!m_Drawer[entityID]) m_Drawer[entityID].reset(new Drawer2D());
				break;
			case ComponentID::ID::FSM:			
				if (!m_FSMs[entityID]) m_FSMs[entityID].reset(new FSM());
				break;
			case ComponentID::ID::Physics:		
				if (!m_Phisics[entityID]) m_Phisics[entityID].reset(new Physics());
				break;
			case ComponentID::ID::SoundPlayer:	
				if (!m_SoundPlayers[entityID]) m_SoundPlayers[entityID].reset(new SoundPlayer());
				break;
			case ComponentID::ID::Transform:	
				if (!m_Transforms[entityID]) m_Transforms[entityID].reset(new Transform2D());
				break;
			}
		}
	}

	void EntitySystem::DetachComponent(int entityID, const std::vector<ComponentID::ID>& components)
	{
		for (auto& id : components)
		{
			switch (id)
			{
			case ComponentID::ID::Animantor:
				m_Animators[entityID].reset();
				break;
			case ComponentID::ID::Controller:
				m_Controllers[entityID].reset();
				break;
			case ComponentID::ID::Drawer:
				m_Drawer[entityID].reset();
				break;
			case ComponentID::ID::FSM:
				m_FSMs[entityID].reset();
				break;
			case ComponentID::ID::Physics:
				m_Phisics[entityID].reset();
				break;
			case ComponentID::ID::SoundPlayer:
				m_SoundPlayers[entityID].reset();
				break;
			case ComponentID::ID::Transform:
				m_Transforms[entityID].reset();
				break;
			}
		}
	}

	void EntitySystem::ActivateComponent(int entityID, const std::vector<ComponentID::ID>& components)
	{
		for (auto& id : components)
		{
			switch (id)
			{
			case ComponentID::ID::Animantor:
				if (!m_Animators[entityID]) m_Animators[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Controller:
				if (!m_Controllers[entityID]) m_Controllers[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Drawer:
				if (!m_Drawer[entityID]) m_Drawer[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::FSM:
				if (!m_FSMs[entityID]) m_FSMs[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Physics:
				if (!m_Phisics[entityID]) m_Phisics[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::SoundPlayer:
				if (!m_SoundPlayers[entityID]) m_SoundPlayers[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Transform:
				if (!m_Transforms[entityID]) m_Transforms[entityID]->m_OnActivate = true;
				break;
			}
		}
	}

	void EntitySystem::InActivateComponent(int entityID, const std::vector<ComponentID::ID>& components)
	{
		for (auto& component : components)
		{
			switch (component)
			{
			case ComponentID::ID::Animantor:
				if (!m_Animators[entityID]) m_Animators[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Controller:
				if (!m_Controllers[entityID]) m_Controllers[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Drawer:
				if (!m_Drawer[entityID]) m_Drawer[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::FSM:
				if (!m_FSMs[entityID]) m_FSMs[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Physics:
				if (!m_Phisics[entityID]) m_Phisics[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::SoundPlayer:
				if (!m_SoundPlayers[entityID]) m_SoundPlayers[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Transform:
				if (!m_Transforms[entityID]) m_Transforms[entityID]->m_OnActivate = false;
				break;
			}
		}
	}

	void EntitySystem::SetFSM(int entityID, const std::initializer_list<std::pair<const EnumType, FSM::InputHandler*>>& handlers)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_FSMs[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have fsm component!", entityID);
			return;
		}
		m_FSMs[entityID]->RegisterFSM(handlers);
	}

	void EntitySystem::SetController(int entityID, const std::initializer_list<Command>& commands)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Controllers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have controller component!", entityID);
			return;
		}
		m_Controllers[entityID]->RegisterCommand(commands);
	}

	void EntitySystem::SetAnimator(int entityID, const std::initializer_list<std::pair<const EnumType, Ref<Animation2D>>>& animationList)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Animators[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have animator component!", entityID);
			return;
		}
		m_Animators[entityID]->RegisterAnimation(animationList);
	}

	void EntitySystem::SetTransform(int entityID, const glm::vec3 & position, const float rotation, const glm::vec2 & scale)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Transforms[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have transform component!", entityID);
			return;
		}
		m_Transforms[entityID]->SetTransform(position, rotation, scale);
	}

	void EntitySystem::SetSoundPlayer(int entityID, const std::initializer_list<std::pair<const EnumType, std::pair<Ref<Sound>, SoundChannel>>>& sounds)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_SoundPlayers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have soundPlayer component!", entityID);
			return;
		}
		m_SoundPlayers[entityID]->RegisterSound(sounds);
	}

	void EntitySystem::DispatchMessage_(int entityID, const std::string message)
	{
		for (auto& entity : m_ActivateEntitys)
		{
			if (entityID != entity.first)
			{
				entity.second->ReceiveMessage_(entityID, message);
			}
		}
	}

}