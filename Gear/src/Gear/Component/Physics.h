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
		bool JudgePixelCollision();

	public:
		void ActivatePixelCollision(const glm::vec3& targetPixel, Ref<Texture2D> collisionTargetTexture, const glm::mat4& textureTranslate);
		inline void ActivateGravity(bool active) { m_ActivatedGravity = active; }
		inline void SetExternalVector(const glm::vec2& externalVector) { m_ExternalVector = externalVector; }
		inline void SetTargetPos(glm::vec3* targetPos) { m_TargetPos = targetPos; }
		void TargetUpdate(Timestep ts);

	private:
		bool m_ActivatedGravity = false;
		bool m_ActivatedPixelCollision = false;

		glm::vec2 m_ExternalVector;
		glm::vec3* m_TargetPos = nullptr;
		glm::vec3 m_TargetPixel;
		glm::mat4 m_PixelCollisionTargetTextureTranslate;

		float m_Gravity;
		float m_Friction;
		float m_Elastics;
		float m_GravityAccelation;
		float m_LimitGravityAccelation;

		Ref<Texture2D> m_PixelCollisionTargetTexture;

		friend class EntitySystem;
	};

}