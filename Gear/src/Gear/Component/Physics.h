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
		
		void RegisterBasicForce(float gravity = 1.0f, float friction = 0.2f, float elastics = 0.3f);

	public:
		inline void SetGravity(bool onGravity) { m_OnGravity = onGravity; }
		inline void SetExternalVector(const glm::vec2& externalVector) { m_ExternalVector = externalVector; }

	private:
		bool m_OnGravity;
		float m_Gravity;
		glm::vec2 m_ExternalVector;

		float m_Friction;
		float m_Elastics;

		friend class EntitySystem;

	};

}