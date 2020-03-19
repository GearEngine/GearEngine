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
			Timer,
			Texturer,
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

		//user interface
	public:
		static int CreateEntity(bool activate = false);
		static Ref<Entity> GetEntity(int entityID);

		static void Update(Timestep ts);
		static void Render();

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
		static void SetPhysics(int entityID, bool activateGravity = false, float gravity = 1.0f, float limitGravityAccelation = 0.0f,  float friction = 0.2f, float elastics = 0.3f);
		static void SetPixelCollision(int entityID, const glm::vec3& targetPixel, Ref<Texture2D> targetTexture, const glm::mat4& targetTextureTranslate, std::vector<std::pair<float, float>> offsets);
		static void SetTexturer(int entityID, RenderType::Type type, Ref<Texture2D> texture, Ref<Texture2D> mask = nullptr, Ref<Texture2D> blending = nullptr);

		static Ref<Transform2D>		GetTransform2D(int entityID);
		static Ref<FSM>				GetFSM(int entityID);
		static Ref<Animator2D>		GetAnimator2D(int entityID);
		static Ref<Physics2D>		GetPhysics2D(int entityID);
		static Ref<SoundPlayer>		GetSoundPlayer(int entityID);
		static Ref<Controller>		GetController(int entityID);
		static Ref<Timer>			GetTimer(int entityID);
		static Ref<Texturer2D>		GetTexturer(int entityID);

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
		static std::vector<Ref<Physics2D>>		m_Phisics;
		static std::vector<Ref<Timer>>			m_Timers;
		static std::vector<Ref<Texturer2D>>		m_Texturer;

		friend class Application;
		friend class EventSystem;
	};

}