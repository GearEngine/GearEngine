#pragma once

#include "WormStatusHandler.h"

namespace InGame {

	class Worm
	{
	public:
		Worm(int teamNumber, int wormNumber, const InitiateData& initData);
		~Worm();

		void Update(Gear::Timestep ts);
		int GetID() { return m_ID; }

	private:
		int m_ID;

		static Gear::Ref<WormExplosionEventHandler> s_ExplosionEventHandler;
		static Gear::Ref<WormWorldEventHandler> s_WorldEventHandler;
	};
}
