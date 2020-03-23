#include "grpch.h"
#include "Physics.h"
#include "Gear/Manager/CoordManager.h"

namespace Gear {

	void Physics2D::Update(Timestep ts)
	{
		m_Timestep = ts;
		TargetUpdate(ts);
	}

	void Physics2D::RegisterBasicForce(float gravity, float limitGravityAccelation, float friction, float elastics)
	{
		m_Gravity = gravity;
		m_LimitGravityAccelation = limitGravityAccelation;
		m_Friction = friction;
		m_Elastics = elastics;
	}

	bool Physics2D::JudgePixelCollision()
	{
		for (auto& offset : m_PixelCollisionOffsetVector)
		{
			auto pixel = Coord2DManger::Get()->GetPixel_From_TextureLocal_With_WorldPosition
				(m_PixelCollisionTargetTexture, glm::vec2(m_TargetPos->x + offset.first, m_TargetPos->y + offset.second), m_PixelCollisionTargetTextureTranslate);
			if (pixel.r == m_TargetPixel.r && pixel.g == m_TargetPixel.g && pixel.b == m_TargetPixel.b)
				return true;
		}
		return false;
	}

	void Physics2D::checkMoveLimit()
	{
		if (m_TargetPos->x < m_MoveLimit.Left)
		{
			m_TargetPos->x = m_MoveLimit.Left;
		}
		if (m_TargetPos->x > m_MoveLimit.Right)
		{
			m_TargetPos->x = m_MoveLimit.Right;
		}
		if (m_TargetPos->y < m_MoveLimit.Bottom)
		{				 
			m_TargetPos->y = m_MoveLimit.Bottom;
		}				 
		if (m_TargetPos->y > m_MoveLimit.Top)
		{				 
			m_TargetPos->y = m_MoveLimit.Top;
		}
	}

	void Physics2D::UpdateSliding()
	{
		if (m_ExternalVector.x + m_ExternalVector.y == 0)
		{
			return;
		}

		m_ExternalVector.x *= m_SlidingRatio;
		m_ExternalVector.y *= m_SlidingRatio;

		if (std::abs(m_ExternalVector.x) + std::abs(m_ExternalVector.y) < 0.05)
		{
			m_ExternalVector.x = 0.0f;
			m_ExternalVector.y = 0.0f;
		}
	}

	void Physics2D::ActivatePixelCollision(const glm::vec3 & targetPixel, Ref<Texture2D> collisionTargetTexture, const glm::mat4& textureTranslate, const std::vector<std::pair<float, float>>& offsets)
	{
		m_TargetPixel = targetPixel;
		m_PixelCollisionTargetTexture = collisionTargetTexture;
		m_PixelCollisionTargetTextureTranslate = textureTranslate;
		m_PixelCollisionOffsetVector = offsets;
		m_ActivatedPixelCollision = true;
	}

	void Physics2D::ActivateMoveLimit(const FRect & moveLimit)
	{
		m_MoveLimit = moveLimit;
		m_ActivatedMoveLimitation = true;
	}

	void Physics2D::TargetUpdate(Timestep ts)
	{
		if (!m_TargetPos)
			return;

		if (m_ActivatedGravity)
		{
			//LimitGravityAccelation 0 is no limit
			if (m_GravityAccelation < m_LimitGravityAccelation || m_LimitGravityAccelation == 0.0f)
			{
				m_GravityAccelation += m_Gravity * ts;
			}
		}
		m_ExternalVector.y -= m_GravityAccelation;

		m_TargetPos->x += m_ExternalVector.x * ts;
		m_TargetPos->y += m_ExternalVector.y * ts;	

		if (m_ActivatedPixelCollision)
		{
			if (JudgePixelCollision())
			{
				m_TargetPos->y -= m_ExternalVector.y * ts;
				m_GravityAccelation = 0.0f;
			}
		}

		if (m_ActivatedMoveLimitation)
		{
			checkMoveLimit();
		}

		if (m_ActivatedSlide)
		{
			UpdateSliding();
		}
	}

	void Physics2D::FollowTarget()
	{
	}

}