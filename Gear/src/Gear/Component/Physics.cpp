#include "grpch.h"
#include "Physics.h"

namespace Gear {

	void Physics2D::Update(Timestep ts)
	{
		m_DeltaTime += ts;
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
		
		return false;
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
				m_TargetPos->x -= m_ExternalVector.x * ts;
				m_TargetPos->y -= m_ExternalVector.y * ts;
			}
		}

	}



}