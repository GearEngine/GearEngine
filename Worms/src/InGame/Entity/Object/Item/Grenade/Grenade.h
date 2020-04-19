#pragma once

#include "../ItemBullet.h"

namespace InGame {

	class Grenade : public ItemBullet
	{
	public:
		Grenade(const InitiateData& initData);
		virtual void init(const glm::vec3& position, float initAngle, float initPower, int From, float explosionTime = 0.0f) override;

	};

}