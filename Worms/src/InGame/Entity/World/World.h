#pragma once

#include "WorldStatusHandler.h"

namespace InGame {

	class World 
	{
	public:
		World(const InitiateData& initData);
		~World();

		void Update(Gear::Timestep ts);
		std::vector<std::string> bgms;

	public:
		static float s_LimitTurnTime;
		static int s_LimitSuddenDeathTurn;
		static int s_CurrentTurn;

	private:
		int m_ID;
	};

}