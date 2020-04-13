#pragma once

namespace InGame {

	class ItemBullet
	{
	public:
		virtual void init(const glm::vec3& position, float initAngle, float initPower, int From) = 0;
		int GetID() const { return m_ID; }

	protected:
		int m_ID;
	};

}
