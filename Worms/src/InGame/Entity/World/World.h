#pragma once

#include "InGame/Data/InitiateData.h"

namespace InGame {

	class World 
	{
	public:
		World(const InitiateData& initData);
		~World();

		void Update(Gear::Timestep ts);
		
	private:
		int m_ID;

	};

}