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
		void InitWormPosition(WormSpecific& wormData, int wormNumber, int teamNumber, Gear::Ref<Gear::Texture2D> mask, const glm::mat4& maskTranslate);
		
		int m_ID;

		static Gear::Ref<WormExplosionEventHandler> s_ExplosionEventHandler;
		static Gear::Ref<WormWorldEventHandler> s_WorldEventHandler;
	};
}
