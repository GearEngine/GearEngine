#pragma once
#include "Gear.h"

#include "InGame/Data/InitiateData.h"
#include "TerrainEventHandler.h"

namespace InGame {

	class Terrain
	{
	public:
		Terrain(const InitiateData& initData);
		~Terrain();

	private:
		int m_ID;
		Gear::Ref<TerrainExplosionEventHandler> m_ExplosionHandler;
	};

}
