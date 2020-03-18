#include "grpch.h"
#include "Timer.h"

namespace Gear {

	void Timer::Update(Timestep ts)
	{
		if (!m_OnProgress || m_OnPause)
			return;

		m_PastTime += ts;
		if (m_SettedTime <= m_PastTime)
		{
			m_PastTime = 0.0f;
			m_OnProgress = false;
			m_OnPause = false;
			m_OnExpired = true;
		}
	}
	void Timer::Pause()
	{
		if (!m_OnProgress)
			return;
		m_OnPause = true;
	}

	void Timer::Start()
	{
		m_PastTime = 0.0f;
		m_OnProgress = true;
		m_OnExpired = false;
		m_OnPause = false;
	}

	void Timer::Stop()
	{
		m_PastTime = 0.0f;
		m_OnProgress = false;
		m_OnPause = false;
		m_OnExpired = true;
	}

	void Timer::Resume()
	{
		m_OnPause = false;
	}

	void Timer::SetTimer(float time)
	{
		if (m_OnProgress)
		{
			GR_CORE_WARN("The Timer is aleady on progress!");
		}
		m_SettedTime = time;
	}

}