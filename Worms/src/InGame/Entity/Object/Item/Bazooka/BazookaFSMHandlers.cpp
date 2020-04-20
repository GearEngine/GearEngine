#include "wmpch.h"
#include "BazookaFSMHandlers.h"

#include "InGame/Layer/BackGroundLayer.h"
#include "InGame/Layer/ObjectLayer.h"

namespace InGame {
	
	Gear::EnumType BazookaOnGoingHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}
		
		auto tick = Gear::EntitySystem::GetTimer(entityID)->GetTick();

		auto externalVector = physics->GetExternalVector();
	
		float angle = Gear::Util::GetAngleFromXY(externalVector.x, externalVector.y);
		int textureIndex = angle / 360.0f * 31.0f - 7;
		if (textureIndex < 0)
		{
			textureIndex += 31;
		}

		if (abs(externalVector.x) <= 0.0001f)
		{
			if (externalVector.y > 0.0f)
			{
				textureIndex = 0;
			}
			else
			{
				textureIndex = 15;
			}
		}
		animator->SetFrameY(textureIndex);
		float currentWind = BackGroundLayer::GetCurrentWind();
		externalVector.x += currentWind * tick * WindAdjustRatio;
		physics->SetExternalVectorX(externalVector.x);

		//Gen Exhaust
		m_pastTime += tick;
		if (m_pastTime > m_GenExhaustDelay)
		{
			m_pastTime = 0.0f;
			auto exhaust = EffectPool::GetExhuast(ExhaustType::_1);

			exhaust->init(Gear::EntitySystem::GetTransform2D(entityID)->GetPosition());
			ObjectLayer::s_Exhausts.push_back(exhaust);
		}

		return Item::State::OnGoing;
	}

	Gear::EnumType BazookaOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}
}


