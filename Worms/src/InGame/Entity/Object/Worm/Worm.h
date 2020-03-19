#pragma once
#include "Gear.h"
#include "WormEventHandle.h"
#include "InGame/Data/InitiateData.h"

namespace InGame {

	class Worm
	{
	public:
		Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale, const InitiateData& initData);
		~Worm();

		void Update(Gear::Timestep ts);
		int GetID() { return m_ID; }

	private:
		int m_ID;

		static Gear::Ref<WormExplosionEventHandler> s_ExplosionHandler;
	};

}
