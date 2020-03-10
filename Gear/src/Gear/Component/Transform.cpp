#include "grpch.h"
#include "Transform.h"

namespace Gear {
	
	Transform2D::Transform2D()
		: m_Translate(glm::mat4(1.0f))
	{
		intialize();
	}
	
	Transform2D::Transform2D(const glm::vec3& position, const glm::vec2& scale, const float rotate)
		:m_Position(position), m_Scale(scale), m_Rotation(rotate), m_Translate(glm::mat4(1.0f))
	{
	}

	void Transform2D::SetPosition(const glm::vec3 & position)
	{
		m_Position = position;
	}

	void Transform2D::SetRotation(const float rotation)
	{
		m_Rotation = rotation;
	}

	void Transform2D::SetScale(const glm::vec2 & scale)
	{
		m_Scale = scale;
	}

	void Transform2D::Move(const glm::vec2 & vector, const Timestep ts)
	{
		//temporary
		m_Position.x += vector.x * ts;
		m_Position.y += vector.y * ts;
	}

	void Transform2D::RecalculateTranslate()
	{
		m_Translate = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), m_Rotation, { 0.0f, 0.0f, 1.0f, })
			* glm::scale(glm::mat4(1.0f), { m_Scale.x, m_Scale.y, 1.0f });
	}
	void Transform2D::intialize()
	{
		m_Position = { 0.f,0.f,0.f };
		m_Scale = { 0.f,0.f };
		m_Rotation = 0.f;
	}

}