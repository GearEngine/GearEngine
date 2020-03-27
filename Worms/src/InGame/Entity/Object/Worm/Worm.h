#pragma once

#include "WormEventHandle.h"

namespace InGame {

	class Worm
	{
	public:
		Worm(int count, const InitiateData& initData);
		~Worm();

		void Update(Gear::Timestep ts);
		int GetID() { return m_ID; }

	private:
		int m_ID;

		static Gear::Ref<WormExplosionEventHandler> s_ExplosionHandler;
	};
}
