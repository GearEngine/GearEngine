#pragma once

#include "Entity.h"
#include "SoundPlayer.h"
#include "Animator.h"
#include "Physics.h"
#include "FSM.h"
#include "Transform.h"
#include "Controller.h"
#include "Drawer.h"

namespace Gear {

	namespace ComponentID
	{
		enum ID : unsigned int
		{
			Animantor,
			Controller,
			Drawer,
			FSM,
			Physics,
			SoundPlayer,
			Transform,
			ComponentMax
		};
	}

	class EntitySystem
	{
	private:
		static void Init();
		static void ShutDown();
		static void Update(Timestep ts);
		static void Render();

		//user interface
	public:
		static int CreateEntity(bool activate = false);

		static void ActivateEntity(int entityID);
		static void InActivateEntity(int entityID);
		static void DeleteEntity(int entityID);

		static void AttachComponent(int entityID, const std::vector<ComponentID::ID>& components);
		static void DetachComponent(int entityID, const std::vector<ComponentID::ID>& components);
		static void ActivateComponent(int entityID, const std::vector<ComponentID::ID>& components);
		static void InActivateComponent(int entityID, const std::vector<ComponentID::ID>& components);

		static void SetFSM(int entityID, const std::initializer_list<std::pair<const EnumType, FSM::InputHandler*>>& handlers);
		static void SetController(int entityID, const std::initializer_list<Command>& commands);
		static void SetAnimator(int entityID, const std::initializer_list < std::pair<const EnumType, Ref<Animation2D>>>& animationList);
		static void SetTransform(int entityID, const glm::vec3& position, const float rotation, const glm::vec2& scale);
		static void SetSoundPlayer(int entityID, const std::initializer_list<std::pair<const EnumType, std::pair<Ref<Sound>, SoundChannel>>>& sounds);
		static void DispatchMessage_(int entityID, const std::string message);

	private:
		static int s_EntityID;

		static std::queue<int> m_SpareIDqueue;

		//first : Entity ID, second Entity Pointer
		static std::unordered_map<int, Ref<Entity>> m_EntityPool;
		static std::unordered_map<int, Ref<Entity>> m_ActivateEntitys;

		//Component Pool
		static std::vector<Ref<Animator2D>>		m_Animators;
		static std::vector<Ref<Controller>>		m_Controllers;
		static std::vector<Ref<FSM>>			m_FSMs;
		static std::vector<Ref<SoundPlayer>>	m_SoundPlayers;
		static std::vector<Ref<Transform2D>>	m_Transforms;
		static std::vector<Ref<Drawer2D>>		m_Drawer;
		static std::vector<Ref<Physics>>		m_Phisics;

		friend class Application;
	};

}