#pragma once
#include "Component.h"
#include "Gear/Core/Timestep.h"

namespace Gear {

	class Transform : public Component
	{
	public:
		virtual ~Transform() = default;

	};

	class Transform2D
	{
	public:
		Transform2D();
		Transform2D(const glm::vec3& position, const glm::vec2& scale, const float rotate);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const float rotation);
		void SetScale(const glm::vec2& scale);
		void Move(const glm::vec2& vector, const Timestep ts);
		
		inline glm::mat4 GetTranslate() { RecalculateTranslate(); return m_Translate; }
		inline glm::vec3 GetPosition() { return m_Position; }
		inline glm::vec2 GetScale() { return m_Scale; }
		inline float GetRotation() { return m_Rotation; }

	private:
		void intialize();
		void RecalculateTranslate();

	private:
		glm::mat4 m_Translate;

		glm::vec3 m_Position;
		glm::vec2 m_Scale;
		float m_Rotation;
	};

}