#pragma once

#include "Entity.h"
#include "SoundPlayer.h"
#include "Animator.h"
#include "Physics.h"
#include "FSM.h"
#include "Transform.h"
#include "Controller.h"
#include "Drawer.h"
#include "Timer.h"
#include "Texturer.h"
#include "EventSystem.h"
#include "Status.h"

namespace Gear {

	namespace ComponentID
	{
		enum ID : unsigned int
		{
			Animator,
			Controller,
			Drawer,
			FSM,
			Physics,
			SoundPlayer,
			Transform,
			Timer,
			Texturer,
			Status,
			LateDrawer,
			ComponentMax
		};
	}

	class EntitySystem
	{
	private:
		static void Init();
		static void Shutdown();
		static void EventHandle(Ref<Entity>& entity);

	private:
		static void UpdateController(int entityID, Timestep ts);
		static void UpdateFSM(int entityID, Timestep ts);
		static void UpdateSoundPlayer(int entityID, Timestep ts);
		static void UpdateDrawer2D(int entityID, Timestep ts);
		static void UpdatePhysics2D(int entityID, Timestep ts);
		static void UpdateTransform2D(int entityID, Timestep ts);
		static void UpdateAnimator2D(int entityID, Timestep ts);
		static void UpdateTimer(int entityID, Timestep ts);
		static void UpdateTexturer2D(int entityID, Timestep ts);
		static void UpdateStatus(int entityID, Timestep ts);
		static void UpdateLateDrawer(int entityID);

		static void InActivateEntity();

		//user interface
	public:
		static int CreateEntity(bool activate = false);
		static Ref<Entity> GetEntity(int entityID);

		static void Update(Timestep ts);
		static void Render();

		static void ActivateEntity(int entityID);
		static void InActivateComponent(int entityID, const std::vector<ComponentID::ID>& components);
		static void DeleteEntity(int entityID);

		static void AttachComponent(int entityID, const std::vector<ComponentID::ID>& components);
		static void DetachComponent(int entityID, const std::vector<ComponentID::ID>& components);
		static void ActivateComponent(int entityID, const std::vector<ComponentID::ID>& components);

		static void SetFSM(int entityID, const std::initializer_list<std::pair<const EnumType, FSM::InputHandler*>>& handlers);
		static void SetController(int entityID, const std::initializer_list<Command>& commands);
		static void SetAnimator(int entityID, const std::initializer_list < std::pair<const EnumType, Ref<Animation2D>>>& animationList);
		static void SetTransform(int entityID, const glm::vec3& position, const float rotation, const glm::vec2& scale);
		static void SetSoundPlayer(int entityID, const std::initializer_list<std::pair<const EnumType, std::pair<Ref<Sound>, SoundChannel>>>& sounds);
		static void SetTexturer(int entityID, RenderType::Type type, Ref<Texture2D> texture, Ref<Texture2D> mask = nullptr, Ref<Texture2D> blending = nullptr);
		static void SetStatus(int entityID, const std::initializer_list<std::pair<const EnumType, std::any>>& statlist);
		static void SetStatusHanlder(int entityID, const std::initializer_list<std::pair<const EnumType, Ref<Status::StatusHandler>>>& handlers);

		static void SetPhysics(int entityID, bool activateGravity = false, float gravity = 1.0f, float limitGravityAccelation = 0.0f,  float friction = 0.2f, float elastics = 0.3f);
		static void SetPixelCollision(int entityID, const glm::vec3& targetPixel, Ref<Texture2D> targetTexture, const glm::mat4& targetTextureTranslate, const std::initializer_list<std::pair<const std::string, Ref<Physics2D::PixelCollisionHander>>>& handlers);
		static void SetMoveLimit(int entityID, const Util::FRect& rect);
		static void SetSliding(int entityID, float slidingRatio);

		static void RegisterInActivateEntity(int entityID);
		static bool IsEntityActivated(int entityID) { return m_EntityPool[entityID]->m_OnActivate; }
		static bool IsComponenetActivate(int entityID, ComponentID::ID componentID);

		static Ref<Transform2D>		GetTransform2D(int entityID);
		static Ref<FSM>				GetFSM(int entityID);
		static Ref<Animator2D>		GetAnimator2D(int entityID);
		static Ref<Physics2D>		GetPhysics2D(int entityID);
		static Ref<SoundPlayer>		GetSoundPlayer(int entityID);
		static Ref<Controller>		GetController(int entityID);
		static Ref<Timer>			GetTimer(int entityID);
		static Ref<Texturer2D>		GetTexturer(int entityID);
		static Ref<Status>			GetStatus(int entityID);
		static Ref<LateDrawer>		GetLateDrawer(int entityID);

	private:
		static int s_EntityID;

		static std::queue<int> m_SpareIDqueue;
		static std::queue<int> m_InActivateQueue;

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
		static std::vector<Ref<Physics2D>>		m_Phisics;
		static std::vector<Ref<Timer>>			m_Timers;
		static std::vector<Ref<Texturer2D>>		m_Texturer;
		static std::vector<Ref<Status>>			m_Status;
		static std::vector<Ref<LateDrawer>>		m_LateDrawers;

		friend class Application;
		friend class EventSystem;
	};

}