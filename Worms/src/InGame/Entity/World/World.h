#pragma once

namespace InGame {

	class World 
	{
	public:
		World();
		~World();

		void Update(Gear::Timestep ts);
		
	private:
		int m_ID;

	};

}