#include "grpch.h"
#include "Physics.h"

namespace Gear {

	void Physics2D::Update(Timestep ts)
	{
		m_DeltaTime += ts;
		TargetUpdate(ts);
	}

	void Physics2D::RegisterBasicForce(float gravity, float friction, float elastics)
	{
		m_Gravity = gravity;
		m_Friction = friction;
		m_Elastics = elastics;
	}

	void Physics2D::TargetUpdate(Timestep ts)
	{
		m_TargetPos->x += m_ExternalVector.x * ts;
		m_TargetPos->y += m_ExternalVector.y * ts;
	}

	void GetPixel()
	{



	}


}