#include "wmpch.h"
#include "Timer.h"

namespace InGame {

	Timer::Timer()
	{
		m_ID = Gear::EntitySystem::CreateEntity(true);
	}

	Timer::~Timer()
	{
	}

}