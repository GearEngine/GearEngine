#pragma once

#include "../ItemBullet.h"

namespace InGame {

	class Bazooka : public ItemBullet
	{
	public:
		Bazooka(const InitiateData& initData);
		virtual void init(const glm::vec3& position, float initAngle, float initPower) override;

	private:
		int m_ID;
	};


}