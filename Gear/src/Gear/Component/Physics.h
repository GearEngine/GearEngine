#pragma once

#include "Component.h"
#include <glm/glm.hpp>

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
		bool JudgePixelCollision();

	public:
		inline void ActiveGravity(bool active) { m_ActivatedGravity = active; }
		inline void SetExternalVector(const glm::vec2& externalVector) { m_ExternalVector = externalVector; }
		inline void SetTargetPos(glm::vec3* targetPos) { m_TargetPos = targetPos; }
		void TargetUpdate(Timestep ts);

	private:
		bool m_ActivatedGravity;
		bool m_ActivatedPixelCollision;
		float m_Gravity;
		glm::vec2 m_ExternalVector;
		glm::vec3* m_TargetPos = nullptr;

		float m_DeltaTime;
		float m_Friction;
		float m_Elastics;
		float m_GravityAccelation;
		float m_LimitGravityAccelation;

		friend class EntitySystem;

	};

}