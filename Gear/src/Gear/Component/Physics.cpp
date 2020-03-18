#include "grpch.h"
#include "Physics.h"
#include "Gear/Manager/CoordManager.h"

namespace Gear {

	void Physics2D::Update(Timestep ts)
	{
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
		auto pixel = Coord2DManger::Get()->GetPixel_From_TextureLocal_With_WorldPosition
		//TODO : Y offset must not hard cording
		(m_PixelCollisionTargetTexture, glm::vec2(m_TargetPos->x, m_TargetPos->y - 0.2f), m_PixelCollisionTargetTextureTranslate);

		if (pixel.r == m_TargetPixel.r && pixel.g == m_TargetPixel.g && pixel.b == m_TargetPixel.b)
			return true;
		return false;
	}

	void Physics2D::ActivatePixelCollision(const glm::vec3 & targetPixel, Ref<Texture2D> collisionTargetTexture, const glm::mat4& textureTranslate)
	{
		m_TargetPixel = targetPixel;
		m_PixelCollisionTargetTexture = collisionTargetTexture;
		m_PixelCollisionTargetTextureTranslate = textureTranslate;
		m_ActivatedPixelCollision = true;
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
	}

}