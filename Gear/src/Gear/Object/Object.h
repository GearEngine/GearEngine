#pragma once

#include <glm/glm.hpp>
#include "Gear/Renderer/Animator.h"

namespace Gear {

	class Object {
		virtual ~Object() = default;

		virtual glm::vec3 GetPosition() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual float GetRotation() const = 0;

		virtual void SetPosition(glm::vec3 position) = 0;
		virtual void SetSize(glm::vec2 size) = 0;
		virtual void SetRotation(float Rotation) = 0;

		virtual void Update(glm::vec2 dirVector = glm::vec2(0.0f, 0.0f), float force = 0) = 0;
		virtual void Render() = 0;

	protected:
		glm::vec3 m_CenterPosition;
		glm::vec2 m_Size;
		float m_Rotation;
	};


	class FrameObject2D : public Object
	{
	public:
		virtual void SetPosition(glm::vec3 position) override;
		virtual void SetSize(glm::vec2 size) override;
		virtual void SetRotation(float rotation) override;

		virtual inline glm::vec3 GetPosition() const override { return m_CenterPosition; }
		virtual inline glm::vec2 GetSize() const override { return m_Size; }
		virtual inline float GetRotation() const override { return m_Rotation; }

		virtual void Update(glm::vec2 dirVector = glm::vec2(0.0f, 0.0f), float force = 0) override;
		virtual void Render() override;



	protected:
		Animator2D m_Animator;
	};

}