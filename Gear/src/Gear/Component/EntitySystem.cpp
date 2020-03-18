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
	std::vector<Ref<Physics2D>>		EntitySystem::m_Phisics = std::vector<Ref<Physics2D>>();
	std::vector<Ref<Timer>>			EntitySystem::m_Timers = std::vector<Ref<Timer>>();

	void EntitySystem::Init()
	{
		m_Animators.resize(10000);
		m_Controllers.resize(10000);
		m_Drawer.resize(10000);
		m_FSMs.resize(10000);
		m_Phisics.resize(10000);
		m_SoundPlayers.resize(10000);
		m_Transforms.resize(10000);
		m_Timers.resize(10000);
	}

	void EntitySystem::Shutdown()
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
		m_Timers.clear();

		EventSystem::Shutdown();
	}

	void EntitySystem::Update(Timestep ts)
	{
		GR_PROFILE_FUNCTION();

		for (auto& entity : m_ActivateEntitys)
		{
			int id = entity.first;
			
			//Event Handle
			EventHandle(entity.second);

			UpdateTimer(id, ts);
			UpdateController(id, ts);
			UpdateFSM(id, ts);
			UpdateTransform2D(id, ts);
			UpdatePhysics2D(id, ts);
			UpdateAnimator2D(id, ts);
			UpdateSoundPlayer(id, ts);
			UpdateDrawer2D(id, ts);
		}
	}

	void EntitySystem::Render()
	{
		for (auto& entity : m_ActivateEntitys)
		{
			int id = entity.first;

			if (!m_Drawer[id] || !m_Drawer[id]->m_OnActivate)
				continue;
			m_Drawer[id]->Render();
		}
	}

	void EntitySystem::EventHandle(Ref<Entity>& entity)
	{
		if (entity->m_EventQueue.empty())
			return;

		auto& event = entity->m_EventQueue.front();
		EntityEventType type = event->Type;

		if (entity->m_EventHandler.find(type) == entity->m_EventHandler.end())
		{
			GR_CORE_WARN("Entity NO.{0} has no event:{1} Handler", entity->m_EntityID, type);
			entity->m_EventQueue.pop();
			return;
		}
		entity->m_EventHandler[type]->Handle(event->Data, entity->m_EntityID);
		entity->m_EventQueue.pop();
	}

	void EntitySystem::UpdateController(int entityID, Timestep ts)
	{
		if (!m_Controllers[entityID] || !m_Controllers[entityID]->m_OnActivate)
		{
			return;
		}
		m_Controllers[entityID]->Update(ts);
	}

	void EntitySystem::UpdateFSM(int entityID, Timestep ts)
	{
		if (!m_FSMs[entityID] || !m_FSMs[entityID]->m_OnActivate)
		{
			return;
		}
		m_FSMs[entityID]->Handle(entityID, m_Controllers[entityID]->GetCommand());
	}

	void EntitySystem::UpdateSoundPlayer(int entityID, Timestep ts)
	{
		if (!m_SoundPlayers[entityID] || !m_SoundPlayers[entityID]->m_OnActivate)
		{
			return;
		}
		m_SoundPlayers[entityID]->Update(ts);
	}

	void EntitySystem::UpdateDrawer2D(int entityID, Timestep ts)
	{
		if (!m_Drawer[entityID] && !m_Drawer[entityID]->m_OnActivate)
		{
			return;
		}
		m_Drawer[entityID]->m_Translate = m_Transforms[entityID]->GetTranslate();
		m_Drawer[entityID]->m_Animation = m_Animators[entityID]->GetCurrentAnimation();
	}

	void EntitySystem::UpdatePhysics2D(int entityID, Timestep ts)
	{
		if (!m_Phisics[entityID] || !m_Phisics[entityID]->m_OnActivate)
		{
			return;
		}
		m_Phisics[entityID]->Update(ts);
	}

	void EntitySystem::UpdateTransform2D(int entityID, Timestep ts)
	{
		if (!m_Transforms[entityID] || !m_Transforms[entityID]->m_OnActivate)
		{
			return;
		}
	}

	void EntitySystem::UpdateAnimator2D(int entityID, Timestep ts)
	{
		if (!m_Animators[entityID] || !m_Animators[entityID]->m_OnActivate)
		{
			return;
		}
		m_Animators[entityID]->Update(ts);
	}

	void EntitySystem::UpdateTimer(int entityID, Timestep ts)
	{
		if (!m_Timers[entityID] || !m_Timers[entityID]->m_OnActivate)
		{
			return;
		}
		m_Timers[entityID]->Update(ts);
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

	Ref<Entity> EntitySystem::GetEntity(int entityID)
	{
		auto entity = m_EntityPool.find(entityID);
		if(entity == m_EntityPool.end()) 
		{
			GR_CORE_TRACE("{0} entity doesn't exist!", entityID);
			return nullptr;
		}
		return entity->second;
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
				if (!m_Phisics[entityID]) m_Phisics[entityID].reset(new Physics2D());
				break;
			case ComponentID::ID::SoundPlayer:	
				if (!m_SoundPlayers[entityID]) m_SoundPlayers[entityID].reset(new SoundPlayer());
				break;
			case ComponentID::ID::Transform:	
				if (!m_Transforms[entityID]) m_Transforms[entityID].reset(new Transform2D());
				break;
			case ComponentID::ID::Timer:
				if (!m_Timers[entityID]) m_Timers[entityID].reset(new Timer());
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
			case ComponentID::ID::Timer:
				m_Timers[entityID].reset();
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
			case ComponentID::ID::Timer:
				if (!m_Timers[entityID]) m_Timers[entityID]->m_OnActivate = true;
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
				if (m_Animators[entityID]) m_Animators[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Controller:
				if (m_Controllers[entityID]) m_Controllers[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Drawer:
				if (m_Drawer[entityID]) m_Drawer[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::FSM:
				if (m_FSMs[entityID]) m_FSMs[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Physics:
				if (m_Phisics[entityID]) m_Phisics[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::SoundPlayer:
				if (m_SoundPlayers[entityID]) m_SoundPlayers[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Transform:
				if (m_Transforms[entityID]) m_Transforms[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Timer:
				if (m_Timers[entityID]) m_Timers[entityID]->m_OnActivate = false;
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

	void EntitySystem::SetPhysics(int entityID, bool activateGravity, float gravity, float limitGravityAccelation, float friction, 
		float elastics, bool activatePixelCollision, const glm::vec3& targetPixel, Ref<Texture2D> targetTexture, 
		const glm::mat4& targetTextureTranslate)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Phisics[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Physics component!", entityID);
			return;
		}
		if (m_Transforms[entityID])
		{
			m_Phisics[entityID]->SetTargetPos(&m_Transforms[entityID]->m_Position);
		}
		m_Phisics[entityID]->RegisterBasicForce(gravity, limitGravityAccelation, friction, elastics);
		m_Phisics[entityID]->ActivateGravity(activateGravity);
		if (activatePixelCollision)
		{
			m_Phisics[entityID]->ActivatePixelCollision(targetPixel, targetTexture, targetTextureTranslate);
		}
	}

	Ref<Transform2D> EntitySystem::GetTransform2DComponent(int entityID)
	{
		if (!m_Transforms[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Transform2D component!", entityID);
			return nullptr;
		}
		return m_Transforms[entityID];
	}

	Ref<Physics> EntitySystem::GetPhysicsComponent(int entityID)
	{
		if (!m_Phisics[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Physics component!", entityID);
			return nullptr;
		}
		return m_Phisics[entityID];
	}

	Ref<FSM> EntitySystem::GetFSMComponent(int entityID)
	{
		if (!m_FSMs[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have FSM component!", entityID);
			return nullptr;
		}
		return m_FSMs[entityID];
	}

	Ref<Animator2D> EntitySystem::GetAnimator2D(int entityID)
	{
		if (!m_Animators[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have FSM component!", entityID);
			return nullptr;
		}
		return m_Animators[entityID];
	}

	Ref<Physics2D> EntitySystem::GetPhysics2D(int entityID)
	{
		if (!m_Phisics[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have FSM component!", entityID);
			return nullptr;
		}
		return m_Phisics[entityID];
	}

	Ref<SoundPlayer> EntitySystem::GetSoundPlayer(int entityID)
	{
		if (!m_SoundPlayers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have FSM component!", entityID);
			return nullptr;
		}
		return m_SoundPlayers[entityID];
	}

	Ref<Controller> EntitySystem::GetController(int entityID)
	{
		if (!m_Controllers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have FSM component!", entityID);
			return nullptr;
		}
		return m_Controllers[entityID];
	}

	Ref<Timer> EntitySystem::GetTimer(int entityID)
	{
		if (!m_Timers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Timer component!", entityID);
			return nullptr;
		}
		return m_Timers[entityID];
	}

}