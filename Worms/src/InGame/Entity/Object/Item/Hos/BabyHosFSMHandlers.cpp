#include "wmpch.h"
#include "BabyHosFSMHandlers.h"

#include "../Item.h"
#include "BabyHos.h"

namespace InGame {

	Gear::EnumType BabyHosOnExplosion::Handle(int entityID, const Gear::Command & cmd)
	{
		Gear::EntitySystem::RegisterInActivateEntity(entityID);
		return Item::State::OnExplosion;
	}

	Gear::EnumType BabyHosOnGoingHandler::Handle(int entityID, const Gear::Command & cmd)
	{
		if (OnAwake)
		{
			Awake(entityID);
		}

		exhaustPastTime += timer->GetTick();
		if (exhaustPastTime > exhaustGenDelay)
		{
			exhaustPastTime = 0.0f;
			auto exhaust = EffectPool::GetExhuast(ExhaustType::Magic);
			exhaust->init(transform->GetPosition());
			EffectPool::pushExhaust(exhaust);
		}

		float& angle = transform->GetRotation();

		if (rotateDir)
		{
			angle += 31.4f * timer->GetTick();
		}
		else
		{
			angle -= 31.4f * timer->GetTick();
		}

		auto& tint = animator->GetCurrentAnimation()->GetTintColor();
		if (r_Down)
		{
			tint.r -= rgbChangeRatio;
			if (tint.r < 0.0f)
			{
				r_Down = !r_Down;
			}
		}
		else
		{
			tint.r += rgbChangeRatio;
			if (tint.r > 1.0f)
			{
				r_Down = !r_Down;
			}
		}

		if (g_Down)
		{
			tint.g -= rgbChangeRatio;
			if (tint.g < 0.0f)
			{
				g_Down = !g_Down;
			}
		}
		else
		{
			tint.g += rgbChangeRatio;
			if (tint.g > 1.0f)
			{
				g_Down = !g_Down;
			}
		}

		if (b_Down)
		{
			tint.b -= rgbChangeRatio;
			if (tint.b < 0.0f)
			{
				b_Down = !b_Down;
			}
		}
		else
		{
			tint.b += rgbChangeRatio;
			if (tint.b > 1.0f)
			{
				b_Down = !b_Down;
			}
		}

		return Item::State::OnGoing;
	}

}