#pragma once

namespace InGame {

	class ItemBullet
	{
	public:
		virtual void init(const glm::vec3& position, float initAngle, float initPower, int From, float explosionTime = 0.0f) = 0;
		int GetID() const { return m_ID; }

	protected:
		int m_ID;
	};

}
