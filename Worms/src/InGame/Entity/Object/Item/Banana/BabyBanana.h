#pragma once

#include "../ItemBullet.h"

namespace InGame {

	class BabyBanana : public ItemBullet
	{
	public:
		BabyBanana(const InitiateData& initData);
		virtual void init(const glm::vec3& position, float initAngle, float initPower, int From, float explosionTime = 0.0f) override;
	};


}