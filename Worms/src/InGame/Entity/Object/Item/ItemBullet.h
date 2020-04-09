#pragma once

namespace InGame {

	class ItemBullet
	{
	public:
		virtual void init(const glm::vec3& position, float initAngle, float initPower) = 0;

	protected:
		int m_ID;
	};

}
