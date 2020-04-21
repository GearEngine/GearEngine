#pragma once
#include "Component.h"
#include "Gear/Core/Timestep.h"

namespace Gear {

	class Transform : public Component
	{
	protected:
		Transform(int id)
			: Component(id)
		{}
	};

	class Transform2D : public Transform
	{
	public:
		Transform2D(int id)
			: Transform(id)
		{
		}
		virtual ~Transform2D();

	public:
		virtual void Update(Timestep ts) override;

		void SetTransform(const glm::vec3& position, const float rotation, const glm::vec2& scale);
		void SetPosition(const glm::vec3& position);
		void SetRotation(const float rotation);
		void SetScale(const glm::vec2& scale);

		glm::mat4& GetTranslate();
		inline glm::vec3& GetPosition() { return m_Position; }
		inline glm::vec2& GetScale() { return m_Scale; }
		inline float& GetRotation() { return m_Rotation; }

	private:
		void intialize();
		void RecalculateTranslate();

	private:
		glm::mat4 m_Translate;
		glm::vec3 m_Position;
		glm::vec2 m_Scale;
		float m_Rotation;

		friend class EntitySystem;
	};

}