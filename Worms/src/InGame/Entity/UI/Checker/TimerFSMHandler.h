#pragma once

#include "InGame/Entity/UI/Checker/TimerEventHandler.h"

namespace InGame {

	const glm::vec3 g_TimerUpPos(-0.91f, -0.89f, ZOrder::z_FlatChecker);
	const glm::vec3 g_TimerDownPos(-0.91f, -1.3f, ZOrder::z_FlatChecker);

	static bool onRed = false;
	static bool onGray = false;

	namespace TimerInfo {

		enum : unsigned int
		{
			TimerCurrentPosition, TimerUpPosition,
			TimerDownPosition,
		};

	}
	
	class TimerOnStartHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto transform = Gear::EntitySystem::GetTransform2D(entityID);
			auto positionY = transform->GetPosition().y;
			Font::PrintFont(glm::vec3(-0.894f, positionY, ZOrder::z_FlatFont), glm::vec3(0.009f * 4, 0.02f * 4, 1.0f), std::to_string(int(World::s_LimitTurnTime)), FontType::GrayNumber, 0.037f);

			auto timer = Gear::EntitySystem::GetTimer(entityID);
			if (timer->isExpired())
			{
				timer->SetTimer(5.0f);
				timer->Start();
				return WorldState::OnPrepareRun;
			}
			
			return WorldState::OnStart;
		}
	};

	class TimerOnPrepareRunHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto timerY = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition().y;
			Font::PrintFont(glm::vec3(-0.894f, timerY, ZOrder::z_FlatFont), glm::vec3(0.009f * 4, 0.02f * 4, 1.0f), std::to_string(int(World::s_LimitTurnTime)), FontType::GrayNumber, 0.037f);
			
			onGray = false;
			onRed = false;

			if (timer->isExpired())
			{
				Gear::EventSystem::DispatchEvent(EventType::World, Gear::EntityEvent(EventType::World, WorldData(RunningStart)));
				return WorldState::OnRunning;
			}
			else
			{
				return WorldState::OnPrepareRun;
			}
		}
	};

	
	class TimerOnRunningHandler : public Gear::FSM::InputHandler
	{
		bool hurrySound = false;
		bool threeBeep = false;
		bool twoBeep = false;
		bool oneBeep = false;
		bool zeroBeep = false;
		
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static const float blinkTerm = 0.3f;
			static float blinkElapsed = 0.0f;
			
			static bool onPressing = false;

			auto timer = Gear::EntitySystem::GetTimer(entityID);
			int remainTime = timer->GetRemainTime();
			auto timerY = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition().y;

			if (remainTime <= 5.0f)
			{
				if (!hurrySound)
				{
					hurrySound = true;
					PLAY_SOUND_NAME("HURRY", WormsSound::wormSpeech);
				}
				if (!onPressing)
				{
					onPressing = true;
					onRed = true;
				}
				blinkElapsed += timer->GetTick();
				if (blinkElapsed > blinkTerm)
				{
					blinkElapsed = 0.0f;
					onRed = !onRed;
				}
			}

			if (remainTime <= 3.0f)
			{
				if (!threeBeep)
				{
					threeBeep = true;
					PLAY_SOUND_NAME("TIMERTICK", WormsSound::effect);
				}
			}
			if (remainTime <= 2.0f)
			{
				if (!twoBeep)
				{
					twoBeep = true;
					PLAY_SOUND_NAME("TIMERTICK", WormsSound::effect);
				}
			}
			if (remainTime <= 1.0f)
			{
				if (!oneBeep)
				{
					oneBeep = true;
					PLAY_SOUND_NAME("TIMERTICK", WormsSound::effect);
				}
			}

			if (onGray)
			{
				Font::PrintFont(glm::vec3(-0.894f, timerY, ZOrder::z_FlatFont), glm::vec3(0.009f * 4, 0.02f * 4, 1.0f), std::to_string(remainTime), FontType::GrayNumber, 0.037f);
			}
			else if (onRed)
			{
				Font::PrintFont(glm::vec3(-0.894f, timerY, ZOrder::z_FlatFont), glm::vec3(0.009f * 4, 0.02f * 4, 1.0f), std::to_string(remainTime), FontType::RedNumber, 0.037f);
			}
			else 
			{
				Font::PrintFont(glm::vec3(-0.894f, timerY, ZOrder::z_FlatFont), glm::vec3(0.009f * 4, 0.02f * 4, 1.0f), std::to_string(remainTime), FontType::WhiteNumber, 0.037f);
			}

			if (timer->isExpired())
			{
				PLAY_SOUND_NAME("BORING", WormsSound::wormSpeech);
				Gear::EntitySystem::GetStatus(entityID)->PushNeedHandleData(TimerStatusHandleType::MoveDown, Gear::Status::StatHandleData(0));
				onPressing = false;
				hurrySound = false;
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::PrepareNextPhase)));
				return WorldState::OnPrepareNextPhase;
			}
			else
			{
				return WorldState::OnRunning;
			}
		}
	};

	class TimerOnPrepareNextPhaseHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (onRed)
			{
				onRed = false;
			}
			/*auto timer = Gear::EntitySystem::GetTimer(entityID);
			if (timer->isExpired())
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
				return WorldState::OnPrepareNextPhase;
			}*/

			return WorldState::OnPrepareNextPhase;
		}
	};

}