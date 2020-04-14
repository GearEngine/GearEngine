#include "wmpch.h"
#include "WorldEventHandler.h"

namespace InGame {

	std::vector<int> WorldWormData::s_LivingWorms = std::vector<int>();
	std::queue<int> WorldWormData::s_WaitingDyeQue = std::queue<int>();

}