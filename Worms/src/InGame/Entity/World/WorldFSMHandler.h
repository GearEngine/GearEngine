#pragma once

#include "WorldEventHandler.h"

namespace InGame {

	class InGemeStartHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			static bool inFirst = true;

			auto timer = Gear::EntitySystem::GetTimer(entityID);
			auto LateDrawer = Gear::EntitySystem::GetLateDrawer(entityID);
			if (inFirst)
			{
				inFirst = false;
				timer->SetTimer(0.5f);
				timer->Start();
			}
			if (timer->isExpired())
			{
				inFirst = true;
				LateDrawer->InActivateStuff("Fog", Gear::Stuff::Quard);
				PLAY_SOUND_NAME("StartRound", WormsSound::effect);
				return WorldState::OnStart;
			}
			LateDrawer->GetQuardStuff("Fog").Color.a -= 0.01f;
			return WorldState::InGameStart;
		}
	};

	class WorldOnStartHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart, entityID)));
			return WorldState::OnRunning;
		}
	};

	class WorldOnPrepareRunHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnPrepareRun;
		}
	};

	class WorldOnWaitingHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			auto timer = Gear::EntitySystem::GetTimer(entityID);

			if (timer->isExpired())
			{
				int damagedWormCount = 0;
				//GR_TRACE("World On Waiting");

				for (int i = 0; i < WorldWormData::s_LivingWorms.size(); ++i)
				{
					auto curState = Gear::EntitySystem::GetFSM(WorldWormData::s_LivingWorms[i])->GetCurrentState();
					if (curState == WormState::OnNothing || curState == WormState::OnAttacked)
					{
						++damagedWormCount;
					}
				}
				if (!damagedWormCount)
				{
					if (WorldWormData::s_WaitingDyeQue.size())
					{
						int WormID = WorldWormData::s_WaitingDyeQue.front();
						auto FSM = Gear::EntitySystem::GetFSM(WormID);
						auto curWormState = FSM->GetCurrentState();

						if (Gear::EntitySystem::IsEntityActivated(WormID))
						{
							if (curWormState == WormState::OnNothing)
							{
								FSM->SetCurrentState(WormState::OnDye);
							}
						}
						else
						{
							WorldWormData::s_WaitingDyeQue.pop();
							if (!WorldWormData::s_WaitingDyeQue.size())
							{
								timer->SetTimer(1.0f);
								timer->Start();
							}
						}
						return WorldState::OnWaiting;
					}
					else
					{
						GR_TRACE("World OnWaiting FSM : Dispatch New start");
						if (Gear::EntitySystem::isNetwork())
						{
							WormsPacket::Event e;
							e.eventType = WormsPacket::Event::EventType::TurnChange;
							Gear::NetWorkManager::Get()->Send(e);
							Gear::NetController::SendOnlyNull = true;
							return WorldState::OnRunning;
						}
						else
						{
							Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
						}
					}
				}
			}
			return WorldState::OnWaiting; 
		}
	};

	class WorldOnRunningHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnRunning;
		}
	};

	class WorldOnItemWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnItemWindow;
		}
	};

	class WorldOnWindowSelectHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnWindowSelect;
		}
	};

	class WorldOnQuitWindowHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WorldState::OnQuitWindow;
		}
	};
	
	class WorldOnVictoryHandler : public Gear::FSM::InputHandler
	{
		bool inFirst = true;
		Gear::Ref<Gear::LateDrawer> lateDrawer;
		Gear::Ref<Gear::Timer> timer;

		float pastTime = 0.0f;
		float fogDelay = 3.0f;

		bool clapPartSound = true;

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (inFirst)
			{
				inFirst = false;
				for (int i = 0; i < WorldWormData::s_ActiveWorms.size(); ++i)
				{
					auto animator = Gear::EntitySystem::GetAnimator2D(WorldWormData::s_ActiveWorms[i]);
					animator->PlayAnimation(WormState::OnVictory);
				}
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, 0, WorldWormData::s_ActiveWorms[0])));
				
				lateDrawer = Gear::EntitySystem::GetLateDrawer(entityID);
				timer = Gear::EntitySystem::GetTimer(entityID);
				lateDrawer->ActivateStuff("Fog", Gear::Stuff::Quard);
				lateDrawer->GetQuardStuff("Fog").Color.a = 0.0f;

				PLAY_SOUND_NAME("VICTORY", WormsSound::wormSpeech);
			}

			if (!IS_PLAYING_SOUND(WormsSound::effect))
			{
				if (clapPartSound)
				{
					PLAY_SOUND_NAME("CrowdPart1", WormsSound::effect);
				}
				else
				{
					PLAY_SOUND_NAME("CrowdPart2", WormsSound::effect);
				}
				clapPartSound = !clapPartSound;
			}

			if (pastTime > fogDelay)
			{
				auto& fog = lateDrawer->GetQuardStuff("Fog");
				fog.Color.a += 0.005f;
				if (fog.Color.a >= 1.0f)
				{
					pastTime = 0.0f;
					inFirst = true;
					return WorldState::OnGameEnd;
				}
			}
			else
			{
				pastTime += timer->GetTick();
			}

			return WorldState::OnGameVictory;
		}
	};

	class WorldOnDrawHandler : public Gear::FSM::InputHandler
	{
		bool inFirst = true;
		Gear::Ref<Gear::LateDrawer> lateDrawer;
		Gear::Ref<Gear::Timer> timer;

		float pastTime = 0.0f;
		float fogDelay = 3.0f;

		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{

			if (inFirst)
			{
				inFirst = false;
				lateDrawer = Gear::EntitySystem::GetLateDrawer(entityID);
				timer = Gear::EntitySystem::GetTimer(entityID);
				lateDrawer->ActivateStuff("Fog", Gear::Stuff::Quard);
				lateDrawer->GetQuardStuff("Fog").Color.a = 0.0f;
			}

			if (pastTime > fogDelay)
			{
				auto& fog = lateDrawer->GetQuardStuff("Fog");
				fog.Color.a += 0.005f;
				if (fog.Color.a >= 1.0f)
				{
					pastTime = 0.0f;
					inFirst = true;
					return WorldState::OnGameEnd;
				}
			}
			else
			{
				pastTime += timer->GetTick();
			}
			return WorldState::OnGameVictory;
		}
	};

	class WorldOnGameEndHandler : public Gear::FSM::InputHandler
	{
		inline virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			Gear::SoundSystem::Get()->AllStop();
			Gear::Application::Get().ActivateEntitySystem(false);
			Gear::SceneManager::Get()->changeScene("MultiScene", -1);

			WorldWormData::s_ActiveWorms.clear();
			WorldWormData::s_LivingWorms.clear();
			while (!WorldWormData::s_WaitingDyeQue.empty())
			{
				WorldWormData::s_WaitingDyeQue.pop();
			}

			return WorldState::OnGameEnd;
		}

	};

}