#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include "Gear/Renderer/Texture.h"

namespace Gear {

	class Physics : public Component
	{

	};

	class Physics2D : public Physics
	{
	public:
		virtual ~Physics2D() = default;

	private:
		virtual void Update(Timestep ts) override;
		
		void RegisterBasicForce(float gravity, float gravityAccelationLimit, float friction, float elastics);
		void ActivatePixelCollision(const glm::vec3& targetPixel, Ref<Texture2D> collisionTargetTexture, const glm::mat4& textureTranslate, const std::vector<std::pair<float, float>>& offsets);
		bool JudgePixelCollision();
		void checkMoveLimit();

	public:
		inline void ActivateGravity() { m_ActivatedGravity = true; }
		inline void InActivateGravity() { m_ActivatedGravity = false; }
		inline void SetExternalVector(const glm::vec2& externalVector) { m_ExternalVector = externalVector; }
		inline void SetTargetPos(glm::vec3* targetPos) { m_TargetPos = targetPos; }
		inline void ActiveSliding(float slidingRatio) { m_ActivatedSlide = true, m_SlidingRatio = slidingRatio; }
		inline void ActivateFollowTarget() { m_ActivatedFollowTarget = true; }
		inline void InActivateFollowTarget() { m_ActivatedFollowTarget = false; m_FollowTarget = nullptr; }
		inline void PauseFollowingTarget() { m_ActivatedFollowTarget = false; }

		void ActivateMoveLimit(const FRect& moveLimit);
		void TargetUpdate(Timestep ts);
		void UpdateSliding();
		void UpdateFollow();
		void SetFollowTarget(glm::vec3* followTargetPos);

	private:
		bool m_ActivatedFlow = false;
		bool m_ActivatedGravity = false;
		bool m_ActivatedSlide = false;
		bool m_ActivatedPixelCollision = false;
		bool m_ActivatedMoveLimitation = false;
		bool m_ActivatedFollowTarget = false;

		glm::vec2 m_ExternalVector;
		glm::vec3* m_TargetPos = nullptr;
		const glm::vec3* m_FollowTarget = nullptr;
		glm::vec3 m_TargetPixel;
		glm::mat4 m_PixelCollisionTargetTextureTranslate;
		std::vector<std::pair<float, float>> m_PixelCollisionOffsetVector;
		FRect m_MoveLimit;

		float m_Timestep;
		float m_Gravity;
		float m_Friction;
		float m_Elastics;
		float m_GravityAccelation;
		float m_SlidingRatio;
		float m_LimitGravityAccelation;

		Ref<Texture2D> m_PixelCollisionTargetTexture;

		friend class EntitySystem;
	};

}