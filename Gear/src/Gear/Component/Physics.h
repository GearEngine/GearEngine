#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include "Gear/Renderer/Texture.h"

namespace Gear {

	class Physics : public Component
	{
	public:
		Physics(int id)
			: Component(id)
		{}
	};

	class Physics2D : public Physics
	{
	public:
		Physics2D(int id)
			: Physics(id)
		{}
		virtual ~Physics2D() = default;

		class PixelCollisionHander
		{
		public:
			virtual void Handle(int entitID) = 0;
			void init(Ref<Texture2D> targetTexutre, glm::vec3* targetPos, glm::vec2* externalVector, glm::mat4* pixelCollisionTargetTextureTranslate, float* gravityAccelation, const glm::vec3& targetPixelColor);
		
		protected:
			Ref<Texture2D> m_TargetTexture;
			float m_TargetTextureWidth;
			float m_TargetTextureHeight;
			class Coord2DManger* s_CoordManager;

			glm::vec3 m_TargetPixelColor;
			glm::vec3* m_TargetPos;
			glm::vec2* m_ExternalVector;
			glm::mat4* m_PixelCollisionTargetTextureTranslate;
			float* m_GravityAccelation;
		};

	private:
		virtual void Update(Timestep ts) override {};
		void Update(int entityID, Timestep ts);
		
		void RegisterBasicForce(float gravity, float gravityAccelationLimit, float friction, float elastics);
		void checkMoveLimit();
		void UpdateSliding();
		void UpdateFollow();

	public:
		void ActivatePixelCollision(const glm::vec3& targetPixel, Ref<Texture2D> collisionTargetTexture, const glm::mat4& textureTranslate, const std::initializer_list<std::pair<const std::string, Ref<Physics2D::PixelCollisionHander>>>& handlers);
		inline void ActivateGravity() { m_ActivatedGravity = true; }
		inline void InActivateGravity() { m_ActivatedGravity = false; }
		inline void SetExternalVector(const glm::vec2& externalVector) { m_ExternalVector = externalVector; }
		inline void SetTargetPos(glm::vec3* targetPos) { m_TargetPos = targetPos; }
		inline void ActiveSliding(float slidingRatio) { m_ActivatedSlide = true, m_SlidingRatio = slidingRatio; }
		inline void ActivateFollowTarget() { m_ActivatedFollowTarget = true; }
		inline void InActivateFollowTarget() { m_ActivatedFollowTarget = false; m_FollowTarget = nullptr; }
		inline void PauseFollowingTarget() { m_ActivatedFollowTarget = false; }
		inline void SetPixelCollisionHandler(const std::string& name) { m_PixelCollisionHandlerName = name; }
		
		void ActivateMoveLimit(const Util::FRect& moveLimit);
		void SetFollowTarget(glm::vec3* followTargetPos);

	private:
		int m_PixelCollisionResult = 0;

		bool m_ActivatedFlow = false;
		bool m_ActivatedGravity = false;
		bool m_ActivatedSlide = false;
		bool m_ActivatedPixelCollision = false;
		bool m_ActivatedMoveLimitation = false;
		bool m_ActivatedFollowTarget = false;

		glm::vec3* m_TargetPos = nullptr;
		const glm::vec3* m_FollowTarget = nullptr;
		glm::vec2 m_ExternalVector = { 0.0f, 0.0f };

		//For pixelCollision
		glm::vec3 m_TargetPixel;
		glm::mat4 m_PixelCollisionTargetTextureTranslate;
		std::unordered_map<std::string, Ref<PixelCollisionHander>> m_PixelCollisionHandlers;
		std::string m_PixelCollisionHandlerName;
		Ref<Texture2D> m_PixelCollisionTargetTexture;

		Util::FRect m_MoveLimit;
		float m_Timestep;
		float m_Gravity;
		float m_Friction;
		float m_Elastics;
		float m_GravityAccelation = 0.0f;
		float m_SlidingRatio;
		float m_LimitGravityAccelation;

		friend class EntitySystem;
	};

}