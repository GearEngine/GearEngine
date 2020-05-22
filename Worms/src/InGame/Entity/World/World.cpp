#include "wmpch.h"
#include "World.h"

namespace InGame {

	float World::s_LimitTurnTime = 0;
	int World::s_LimitSuddenDeathTurn = 0;
	int World::s_CurrentTurn = 0;

	World::World(const InitiateData& initData)
	{
		s_LimitSuddenDeathTurn = initData.LimitSuddenDeathTurn;
		s_LimitTurnTime = initData.LimitTurnTime;
		s_CurrentTurn = 0;

		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true, "World");

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::FSM,	Gear::ComponentID::Status, Gear::ComponentID::Timer,
			Gear::ComponentID::LateDrawer
		});

		//Set Component specific
		Gear::EntitySystem::SetFSM(m_ID, {
			{ WorldState::InGameStart, new InGemeStartHandler }, { WorldState::OnStart, new WorldOnStartHandler },
			{ WorldState::OnPrepareRun, new WorldOnPrepareRunHandler }, { WorldState::OnRunning, new WorldOnRunningHandler},
			{ WorldState::OnQuitWindow, new WorldOnQuitWindowHandler},	{WorldState::OnWaiting, new WorldOnWaitingHandler },
			{ WorldState::OnGameVictory, new WorldOnVictoryHandler }, { WorldState::OnGameDraw, new WorldOnDrawHandler },
			{WorldState::OnGameEnd, new WorldOnGameEndHandler }
		});
		Gear::EntitySystem::GetFSM(m_ID)->SetCurrentState(WorldState::InGameStart);

		Gear::EntitySystem::SetStatus(m_ID, {
			{ WorldInfo::CurrentWormName, std::string("") }, { WorldInfo::CurrentWormID, -1},
			{ WorldInfo::DyeInfo, std::stack<int>()},
			{ WorldInfo::CurrnetTeam, std::string("") }, { WorldInfo::CurrentTeamColor, TeamColor::Blue },
			{ WorldInfo::TeamInfo, initData.Teams },	 { WorldInfo::TeamInfoBlink, false }
		});

		Gear::EntitySystem::SetStatusHanlder(m_ID, {
			{ WorldStatusHandleType::DisplayWaitingCount, Gear::CreateRef<WorldDisplayWaitingCountHandler>() },
			{ WorldStatusHandleType::DisplayTeamInfo, Gear::CreateRef<WorldDisplayTeamInfoHandler>() },
			{ WorldStatusHandleType::DisplayMassage, Gear::CreateRef<WorldDisplayMasageHandler>() },
		});

		auto status = Gear::EntitySystem::GetStatus(m_ID);
		status->PushNeedHandleData(WorldStatusHandleType::DisplayTeamInfo,
			Gear::Status::StatHandleData(WorldTeamInfoDenoteData(Gear::TextureStorage::GetTexture2D("WormNameBorder"))));

		auto lateDrawer = Gear::EntitySystem::GetLateDrawer(m_ID);
	
		glm::mat4 fogTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1.0f));
		lateDrawer->UpLoadDrawStuff("Fog", Gear::LateDrawer::QuardStuff(fogTranslate, glm::vec4(0.0f, 0.0f, 0.001f, 1.0f)));
		
		//Subscpribe EventChannel
		//Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseClick);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::World, Gear::CreateRef<WorldEventHandler>());

		bgms.push_back("ingame-01-generic");
		bgms.push_back("ingame-02-cavern");
		bgms.push_back("ingame-03-jungle");
		bgms.push_back("ingame-04-battlezone");
		bgms.push_back("ingame-05-forest");
		bgms.push_back("ingame-06-weird-alien-plan");
		bgms.push_back("ingame-07-outerspace");
		bgms.push_back("ingame-08-desert");
		bgms.push_back("ingame-09-hell");
		bgms.push_back("ingame-10-mech-workshop");
		bgms.push_back("ingame-11-rain&surf");
	}

	World::~World()
	{
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::MouseClick);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void World::Update(Gear::Timestep ts)
	{
		if (!IS_PLAYING_SOUND(WormsSound::bgm))
		{
			PLAY_SOUND_NAME(bgms[Gear::Util::GetRndInt(11)], WormsSound::bgm);
			Gear::SoundSystem::Get()->SetVolue(WormsSound::bgm, 0.4f);
		}
	}

}