#include "grpch.h"
#include "Physics.h"
#include "Gear/Manager/CoordManager.h"

namespace Gear {

	void Physics2D::Update(int entityID, Timestep ts)
	{
		m_Timestep = ts;
		
		if (!m_TargetPos)
			return;

		if (m_ActivatedGravity)
		{
			//LimitGravityAccelation 0 is no limit
			if (m_GravityAccelation < 0.0f)
			{
				m_ExternalVector.y -= m_Gravity * ts;
			}
			else if (m_GravityAccelation < m_LimitGravityAccelation || m_LimitGravityAccelation == 0.0f)
			{
				m_GravityAccelation += m_Gravity * ts;
				m_ExternalVector.y -= m_GravityAccelation;
			}
			else if (m_GravityAccelation > m_LimitGravityAccelation)
			{
				m_GravityAccelation = m_LimitGravityAccelation;
				m_ExternalVector.y -= m_GravityAccelation;
			}
		}

		if (m_ActivatedFollowTarget)
		{
			UpdateFollow();
		}

		m_TargetPos->x += m_ExternalVector.x * ts;
		m_TargetPos->y += m_ExternalVector.y * ts;


		if (m_ActivatedPixelCollision)
		{
			m_PixelCollisionHandlers[m_PixelCollisionHandlerName]->Handle(entityID);
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

	void Physics2D::RegisterBasicForce(float gravity, float limitGravityAccelation, float friction, float elastics)
	{
		m_Gravity = gravity;
		m_LimitGravityAccelation = limitGravityAccelation;
		m_Friction = friction;
		m_Elastics = elastics;
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

	void Physics2D::SetFollowTarget(glm::vec3 * followTargetPos)
	{
		m_FollowTarget = followTargetPos;
	}

	void Physics2D::ActivatePixelCollision(const glm::vec3 & targetPixel, Ref<Texture2D> collisionTargetTexture, const glm::mat4& textureTranslate, const std::initializer_list<std::pair<const std::string, Ref<Physics2D::PixelCollisionHander>>>& handlers)
	{
		m_TargetPixel = targetPixel;
		m_PixelCollisionTargetTexture = collisionTargetTexture;
		m_PixelCollisionTargetTextureTranslate = textureTranslate;
		m_PixelCollisionHandlers = handlers;
		m_ActivatedPixelCollision = true;

		for (auto& handler : m_PixelCollisionHandlers)
		{
			handler.second->init(m_PixelCollisionTargetTexture, m_TargetPos, &m_ExternalVector, &m_ActivatedGravity, &m_PixelCollisionTargetTextureTranslate, &m_GravityAccelation, m_TargetPixel);
		}
	}

	void Physics2D::ActivateMoveLimit(const Util::FRect & moveLimit)
	{
		m_MoveLimit = moveLimit;
		m_ActivatedMoveLimitation = true;
	}

	void Physics2D::UpdateFollow()
	{
		if (!m_FollowTarget)
		{
			GR_CORE_WARN("On Physics2D::UpdateFollow : There is no Follow Target!");
			return;
		}

		m_ExternalVector.x = (m_FollowTarget->x - m_TargetPos->x) * 5;
		m_ExternalVector.y = (m_FollowTarget->y - m_TargetPos->y) * 5;
	}

	void Physics2D::PixelCollisionHander::init(Ref<Texture2D> targetTexutre, glm::vec3 * targetPos, glm::vec2 * externalVector, bool* activatedGravity, glm::mat4 * pixelCollisionTargetTextureTranslate, float * gravityAccelation, const glm::vec3& targetPixelColor)
	{
		m_TargetTexture = targetTexutre;
		m_TargetPos = targetPos;
		m_ExternalVector = externalVector;
		m_PixelCollisionTargetTextureTranslate = pixelCollisionTargetTextureTranslate;
		m_GravityAccelation = gravityAccelation;
		m_TargetPixelColor = targetPixelColor;

		m_ActivatedGravity = activatedGravity;
		m_TargetTextureWidth = targetTexutre->GetWidth();
		m_TargetTextureHeight = targetTexutre->GetHeight();
		s_CoordManager = Coord2DManger::Get();
	}

}