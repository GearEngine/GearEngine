#pragma once

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