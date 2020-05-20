#include "grpch.h"
#include "EntitySystem.h"

namespace Gear {

	int EntitySystem::s_EntityID = 0;
	bool EntitySystem::s_isNetWork = false;

	std::queue<int> EntitySystem::m_SpareIDqueue = std::queue<int>();
	std::queue<int> EntitySystem::m_InActivateQueue = std::queue<int>();

	std::unordered_map<int, Ref<Entity>> EntitySystem::m_EntityPool = std::unordered_map<int, Ref<Entity>>();
	std::unordered_map<int, Ref<Entity>> EntitySystem::m_ActivateEntitys = std::unordered_map<int, Ref<Entity>>();

	std::vector<Ref<Animator2D>>	EntitySystem::m_Animators = std::vector<Ref<Animator2D>>();
	std::vector<Ref<Controller>>	EntitySystem::m_Controllers = std::vector<Ref<Controller>>();
	std::vector<Ref<NetController>>	EntitySystem::m_NetControllers = std::vector<Ref<NetController>>();
	std::vector<Ref<FSM>>			EntitySystem::m_FSMs = std::vector<Ref<FSM>>();
	std::vector<Ref<SoundPlayer>>	EntitySystem::m_SoundPlayers = std::vector<Ref<SoundPlayer>>();
	std::vector<Ref<Transform2D>>	EntitySystem::m_Transforms = std::vector<Ref<Transform2D>>();
	std::vector<Ref<Drawer2D>>		EntitySystem::m_Drawer = std::vector<Ref<Drawer2D>>();
	std::vector<Ref<Physics2D>>		EntitySystem::m_Phisics = std::vector<Ref<Physics2D>>();
	std::vector<Ref<Timer>>			EntitySystem::m_Timers = std::vector<Ref<Timer>>();
	std::vector<Ref<Texturer2D>>	EntitySystem::m_Texturer = std::vector<Ref<Texturer2D>>();
	std::vector<Ref<Status>>		EntitySystem::m_Status = std::vector<Ref<Status>>();
	std::vector<Ref<LateDrawer>>	EntitySystem::m_LateDrawers = std::vector<Ref<LateDrawer>>();

	void EntitySystem::Init()
	{
		srand(time(NULL));

		m_Animators.resize(10000);
		m_Controllers.resize(10000);
		m_NetControllers.resize(10000);
		m_Drawer.resize(10000);
		m_FSMs.resize(10000);
		m_Phisics.resize(10000);
		m_SoundPlayers.resize(10000);
		m_Transforms.resize(10000);
		m_Timers.resize(10000);
		m_Texturer.resize(10000);
		m_Status.resize(10000);
		m_LateDrawers.resize(10000);

	}

	void EntitySystem::Shutdown()
	{
		s_EntityID = 0;
		while (!m_SpareIDqueue.empty())
		{
			m_SpareIDqueue.pop();
		}
		while (!m_InActivateQueue.empty())
		{
			m_InActivateQueue.pop();
		}

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
		m_Texturer.clear();
		m_Status.clear();
		m_LateDrawers.clear();
		m_NetControllers.clear();

		EventSystem::Shutdown();
	}

	void EntitySystem::Update(Timestep ts)
	{
		GR_PROFILE_FUNCTION();

		InActivateEntity();

		if (s_isNetWork)
		{
			for (auto& entity : m_ActivateEntitys)
			{
				int id = entity.first;
				NetControllerReceive(id, ts);
			}
		}

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
			UpdateStatus(id, ts);
			UpdateTexturer2D(id, ts);
			UpdateAnimator2D(id, ts);
			UpdateSoundPlayer(id, ts);
			UpdateDrawer2D(id, ts);
		}

		if (s_isNetWork)
		{
			for (auto& entity : m_ActivateEntitys)
			{
				int id = entity.first;
				NetControllerSend(id, ts);
			}
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
		for(auto& entity : m_ActivateEntitys)
		{
			int id = entity.first;
			UpdateLateDrawer(id);
		}

	}

	void EntitySystem::EventHandle(Ref<Entity>& entity)
	{
		if (entity->m_EventQueue.empty())
			return;

		for (auto event = entity->m_EventQueue.begin(); event != entity->m_EventQueue.end(); )
		{
			EntityEventType type = event->Type;

			//has no handler
			if (entity->m_EventHandler.find(type) == entity->m_EventHandler.end())
			{
				event = entity->m_EventQueue.erase(event);
				continue;
			}

			if (!entity->m_EventHandler[type])
			{
				GR_CORE_WARN("There is no Event handler");
				return;
			}

			entity->m_EventHandler[type]->Handle(event->Data, entity->m_EntityID, event->handled);
			if (event->handled)
			{
				event = entity->m_EventQueue.erase(event);
			}
			else
			{
				event++;
			}
		}
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

		if (m_Controllers[entityID] && !m_NetControllers[entityID])
		{
			m_FSMs[entityID]->Handle(entityID, m_Controllers[entityID]->GetCommand());
			m_Controllers[entityID]->ResetCommand();
		}
		else if (!m_Controllers[entityID] && m_NetControllers[entityID])
		{
			m_FSMs[entityID]->Handle(entityID, m_NetControllers[entityID]->GetCommand());
			m_NetControllers[entityID]->ResetCommand();
		}
		else if (m_Controllers[entityID] && m_NetControllers[entityID])
		{
			auto userCmd = m_Controllers[entityID]->GetCommand();
			auto netCmd = m_NetControllers[entityID]->GetCommand();

			if (userCmd == Controller::s_None)
			{
				m_FSMs[entityID]->Handle(entityID, netCmd);
			}
			else if (netCmd == Controller::s_None)
			{
				m_FSMs[entityID]->Handle(entityID, userCmd);
			}
			else
			{
				m_FSMs[entityID]->Handle(entityID, netCmd);
			}
			m_Controllers[entityID]->ResetCommand();
			m_NetControllers[entityID]->ResetCommand();
		}
		else
		{
			m_FSMs[entityID]->Handle(entityID, Controller::s_None);
		}
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
		auto& drawer = m_Drawer[entityID];
		if (!drawer)
		{
			return;
		}
		if (!drawer->m_OnActivate)
		{
			return;
		}
		if (m_Transforms[entityID])
		{
			drawer->m_Translate = m_Transforms[entityID]->GetTranslate();
		}
		if (m_Animators[entityID])
		{
			drawer->m_Animation = m_Animators[entityID]->GetCurrentAnimation();
			return;
		}

		auto& texturer = m_Texturer[entityID];
		if (texturer)
		{
			drawer->m_Texture = texturer->m_Texture;
			drawer->m_RenderType = texturer->m_RenderType;
			switch (drawer->m_RenderType)
			{
			case RenderType::Masking:
				drawer->m_Mask = texturer->m_Mask;
				break;
			case RenderType::Blending:
				drawer->m_Blending = texturer->m_BlendTexture;
				break;
			}
		}
	}

	void EntitySystem::UpdatePhysics2D(int entityID, Timestep ts)
	{
		if (!m_Phisics[entityID] || !m_Phisics[entityID]->m_OnActivate)
		{
			return;
		}
		m_Phisics[entityID]->Update(entityID, ts);
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

	void EntitySystem::UpdateTexturer2D(int entityID, Timestep ts)
	{
		if (!m_Texturer[entityID] || !m_Texturer[entityID]->m_OnActivate)
		{
			return;
		}
		m_Texturer[entityID]->Update(ts);
	}

	void EntitySystem::UpdateStatus(int entityID, Timestep ts)
	{
		if (!m_Status[entityID] || !m_Status[entityID]->m_OnActivate)
		{
			return;
		}
		m_Status[entityID]->Update(ts);
	}

	void EntitySystem::UpdateLateDrawer(int entityID)
	{
		if (!m_LateDrawers[entityID] || !m_LateDrawers[entityID]->m_OnActivate)
		{
			return;
		}
		m_LateDrawers[entityID]->Render();
	}

	void EntitySystem::NetControllerSend(int entityID, Timestep ts)
	{
		if (!m_NetControllers[entityID] || !m_NetControllers[entityID]->m_OnActivate)
		{
			return;
		}
		m_NetControllers[entityID]->SendInput();
	}

	void EntitySystem::NetControllerReceive(int entityID, Timestep ts)
	{
		if (!m_NetControllers[entityID] || !m_NetControllers[entityID]->m_OnActivate)
		{
			return;
		}
		m_NetControllers[entityID]->ReceiveInput();
	}

	int EntitySystem::CreateEntity(bool activate, const std::string& name)
	{
		int entityID;
		Ref<Entity> newEntity;

		if (m_SpareIDqueue.empty())
		{
			entityID = s_EntityID++;

			newEntity.reset(new Entity(entityID, name));

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

			newEntity.reset(new Entity(entityID, name));

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
		if (entity == m_EntityPool.end())
		{
			GR_CORE_TRACE("{0} entity doesn't exist!", entityID);
			return nullptr;
		}
		return entity->second;
	}

	void EntitySystem::isNetwork(bool isNetWork)
	{
		s_isNetWork = isNetWork;
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

	void EntitySystem::InActivateEntity()
	{
		while (!m_InActivateQueue.empty())
		{
			int entityID = m_InActivateQueue.front();
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
			m_InActivateQueue.pop();
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
			m_NetControllers[entityID].reset();
			m_Drawer[entityID].reset();
			m_FSMs[entityID].reset();
			m_Phisics[entityID].reset();
			m_SoundPlayers[entityID].reset();
			m_Transforms[entityID].reset();
			m_Texturer[entityID].reset();
			m_Status[entityID].reset();
			m_LateDrawers[entityID].reset();
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
			case ComponentID::ID::Animator:
				if (!m_Animators[entityID]) m_Animators[entityID].reset(new Animator2D(entityID));
				break;
			case ComponentID::ID::Controller:
				if (!m_Controllers[entityID]) m_Controllers[entityID].reset(new Controller(entityID));
				break;
			case ComponentID::ID::NetController:
				if (!m_NetControllers[entityID]) m_NetControllers[entityID].reset(new NetController(entityID));
				break;
			case ComponentID::ID::Drawer:
				if (!m_Drawer[entityID]) m_Drawer[entityID].reset(new Drawer2D(entityID));
				break;
			case ComponentID::ID::FSM:
				if (!m_FSMs[entityID]) m_FSMs[entityID].reset(new FSM(entityID));
				break;
			case ComponentID::ID::Physics:
				if (!m_Phisics[entityID]) m_Phisics[entityID].reset(new Physics2D(entityID));
				break;
			case ComponentID::ID::SoundPlayer:
				if (!m_SoundPlayers[entityID]) m_SoundPlayers[entityID].reset(new SoundPlayer(entityID));
				break;
			case ComponentID::ID::Transform:
				if (!m_Transforms[entityID]) m_Transforms[entityID].reset(new Transform2D(entityID));
				break;
			case ComponentID::ID::Timer:
				if (!m_Timers[entityID]) m_Timers[entityID].reset(new Timer(entityID));
				break;
			case ComponentID::ID::Texturer:
				if (!m_Texturer[entityID]) m_Texturer[entityID].reset(new Texturer2D(entityID));
				break;
			case ComponentID::ID::Status:
				if (!m_Status[entityID]) m_Status[entityID].reset(new Status(entityID));
				break;
			case ComponentID::ID::LateDrawer:
				if (!m_LateDrawers[entityID]) m_LateDrawers[entityID].reset(new LateDrawer(entityID));
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
			case ComponentID::ID::Animator:
				m_Animators[entityID].reset();
				break;
			case ComponentID::ID::Controller:
				m_Controllers[entityID].reset();
				break;
			case ComponentID::ID::NetController:
				m_NetControllers[entityID].reset();
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
			case ComponentID::ID::Texturer:
				m_Texturer[entityID].reset();
				break;
			case ComponentID::ID::Status:
				m_Status[entityID].reset();
				break;
			case ComponentID::ID::LateDrawer:
				m_LateDrawers[entityID].reset();
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
			case ComponentID::ID::Animator:
				if (m_Animators[entityID]) m_Animators[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Controller:
				if (m_Controllers[entityID]) m_Controllers[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::NetController:
				if (m_NetControllers[entityID]) m_NetControllers[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Drawer:
				if (m_Drawer[entityID]) m_Drawer[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::FSM:
				if (m_FSMs[entityID]) m_FSMs[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Physics:
				if (m_Phisics[entityID]) m_Phisics[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::SoundPlayer:
				if (m_SoundPlayers[entityID]) m_SoundPlayers[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Transform:
				if (m_Transforms[entityID]) m_Transforms[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Timer:
				if (m_Timers[entityID]) m_Timers[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Texturer:
				if (m_Texturer[entityID]) m_Texturer[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::Status:
				if (m_Status[entityID]) m_Status[entityID]->m_OnActivate = true;
				break;
			case ComponentID::ID::LateDrawer:
				if (m_LateDrawers[entityID]) m_LateDrawers[entityID]->m_OnActivate = true;
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
			case ComponentID::ID::Animator:
				if (m_Animators[entityID]) m_Animators[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Controller:
				if (m_Controllers[entityID]) m_Controllers[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::NetController:
				if (m_NetControllers[entityID]) m_NetControllers[entityID]->m_OnActivate = false;
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
			case ComponentID::ID::Texturer:
				if (!m_Texturer[entityID]) m_Texturer[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::Status:
				if (!m_Status[entityID]) m_Status[entityID]->m_OnActivate = false;
				break;
			case ComponentID::ID::LateDrawer:
				if (!m_LateDrawers[entityID]) m_LateDrawers[entityID]->m_OnActivate = false;
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

	void EntitySystem::SetNetController(int entityID, const std::initializer_list<Command>& commands)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_NetControllers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have NetController component!", entityID);
			return;
		}
		m_NetControllers[entityID]->RegisterCommand(commands);
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

	void EntitySystem::SetPhysics(int entityID, bool activateGravity, float gravity, float limitGravityAccelation, float friction, float elastics)
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
		if (activateGravity)
		{
			m_Phisics[entityID]->ActivateGravity();
		}
		m_Phisics[entityID]->RegisterBasicForce(gravity, limitGravityAccelation, friction, elastics);
	}

	void EntitySystem::SetPixelCollision(int entityID, const glm::vec3 & targetPixel, Ref<Texture2D> targetTexture, const glm::mat4 & targetTextureTranslate, const std::initializer_list<std::pair<const std::string, Ref<Physics2D::PixelCollisionHander>>>& handlers)
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
		m_Phisics[entityID]->ActivatePixelCollision(targetPixel, targetTexture, targetTextureTranslate, handlers);
	}

	void EntitySystem::SetMoveLimit(int entityID, const Util::FRect & rect)
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
		m_Phisics[entityID]->ActivateMoveLimit(rect);
	}

	void EntitySystem::SetSliding(int entityID, float slidingRatio)
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
		m_Phisics[entityID]->ActiveSliding(slidingRatio);
	}

	void EntitySystem::RegisterInActivateEntity(int entityID)
	{
		m_InActivateQueue.push(entityID);
	}

	bool EntitySystem::IsComponenetActivate(int entityID, ComponentID::ID componentID)
	{
		switch (componentID)
		{
		case Gear::ComponentID::Animator:
			if (m_Animators[entityID]) return m_Animators[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Controller:
			if (m_Controllers[entityID]) return m_Controllers[entityID]->IsActivate();
			break;
		case Gear::ComponentID::NetController:
			if (m_NetControllers[entityID]) return m_NetControllers[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Drawer:
			if (m_Drawer[entityID]) return m_Drawer[entityID]->IsActivate();
			break;
		case Gear::ComponentID::FSM:
			if (m_FSMs[entityID]) return m_FSMs[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Physics:
			if (m_Phisics[entityID]) return m_Phisics[entityID]->IsActivate();
			break;
		case Gear::ComponentID::SoundPlayer:
			if (m_SoundPlayers[entityID]) return m_SoundPlayers[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Transform:
			if (m_Transforms[entityID]) return m_Transforms[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Timer:
			if (m_Timers[entityID]) return m_Timers[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Texturer:
			if (m_Texturer[entityID]) return m_Texturer[entityID]->IsActivate();
			break;
		case Gear::ComponentID::Status:
			if (m_Status[entityID]) return m_Status[entityID]->IsActivate();
			break;
		case Gear::ComponentID::LateDrawer:
			if (m_LateDrawers[entityID]) return m_LateDrawers[entityID]->IsActivate();
			break;
		}
		GR_CORE_WARN("{0} entity has no {1} component!", entityID, componentID);
		return false;
	}

	int EntitySystem::GetEntityIDFromName(const std::string & name)
	{
		for (auto& entity : m_EntityPool)
		{
			if (entity.second->m_Name == name)
			{
				return entity.first;
			}
		}

		return -1;
	}

	void EntitySystem::SetTexturer(int entityID, RenderType::Type type, Ref<Texture2D> texture, Ref<Texture2D> mask, Ref<Texture2D> blending)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Texturer[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Physics component!", entityID);
			return;
		}
		m_Texturer[entityID]->RegisterTexture(type, texture, mask, blending);

	}

	void EntitySystem::SetStatus(int entityID, const std::initializer_list<std::pair<const EnumType, std::any>>& statuslist)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Status[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Status component!", entityID);
			return;
		}
		m_Status[entityID]->RegisterStatus(statuslist);
	}

	void EntitySystem::SetStatusHanlder(int entityID, const std::initializer_list<std::pair<const EnumType, Ref<Status::StatusHandler>>>& handlers)
	{
		auto entity = m_EntityPool.find(entityID);
		if (entity == m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		if (!m_Status[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Status component!", entityID);
			return;
		}
		m_Status[entityID]->RegisterStatusHandler(handlers);
	}

	Ref<Transform2D> EntitySystem::GetTransform2D(int entityID)
	{
		if (!m_Transforms[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Transform2D component!", entityID);
			return nullptr;
		}
		return m_Transforms[entityID];
	}

	Ref<FSM> EntitySystem::GetFSM(int entityID)
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
			GR_CORE_WARN("{0} entity doesn't have Controller component!", entityID);
			return nullptr;
		}
		return m_Controllers[entityID];
	}

	Ref<NetController> EntitySystem::GetNetController(int entityID)
	{
		if (!m_NetControllers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have NetController component!", entityID);
			return nullptr;
		}
		return m_NetControllers[entityID];
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

	Ref<Texturer2D> EntitySystem::GetTexturer(int entityID)
	{
		if (!m_Texturer[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Timer component!", entityID);
			return nullptr;
		}
		return m_Texturer[entityID];
	}

	Ref<Status> EntitySystem::GetStatus(int entityID)
	{
		if (!m_Status[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have Status component!", entityID);
			return nullptr;
		}
		return m_Status[entityID];
	}

	Ref<LateDrawer> EntitySystem::GetLateDrawer(int entityID)
	{
		if (!m_LateDrawers[entityID])
		{
			GR_CORE_WARN("{0} entity doesn't have LateDrawer component!", entityID);
			return nullptr;
		}
		return m_LateDrawers[entityID];
	}
	
}
