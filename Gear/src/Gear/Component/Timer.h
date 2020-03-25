#pragma once

#include "Gear/Component/Component.h"

namespace Gear {

	class Timer : public Component
	{
	private:
		virtual void Update(Timestep ts);

	public:
		void Pause();
		void Start();
		void Stop();
		void Resume();
		void SetTimer(float time);

		inline float GetTick() const { return m_Tick; }
		inline float GetElapsedTime() const { return m_ElapsedTime; }
		inline float GetRemainTime() const { return m_SettedTime - m_ElapsedTime; }
		inline bool isOnProgress() const { return m_OnProgress; }
		inline bool isExpired() const { return m_OnExpired; }

	private:
		float m_SettedTime = 0.0f;
		float m_ElapsedTime = 0.0f;
		float m_Tick = 0.0f;

		bool m_OnProgress = false;
		bool m_OnPause = false;
		bool m_OnExpired = false;

		friend class EntitySystem;
	};


}