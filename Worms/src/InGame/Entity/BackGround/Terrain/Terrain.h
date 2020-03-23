#pragma once
#include "InGame/Data/InitiateData.h"


namespace InGame {

	class Terrain
	{
	public:
		Terrain(const InitiateData& initData);
		~Terrain();

	private:
		int m_ID;
	};

}
