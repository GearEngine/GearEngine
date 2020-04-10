#include "wmpch.h"
#include "ObjectLayer.h"

#include "InGame/Entity/Object/Worm/WormEnum.h"
#include "InGame/Entity/Object/Item/Item.h"

namespace InGame {

	std::unordered_map<std::string, std::vector<Gear::Ref<Worm>>> ObjectLayer::s_Worms = std::unordered_map<std::string, std::vector<Gear::Ref<Worm>>>();
	std::unordered_map<std::string, TeamInfo> ObjectLayer:: s_TeamInfo = std::unordered_map<std::string, TeamInfo>();
	std::unordered_map<std::string, TeamInfo>::iterator ObjectLayer:: s_TeamIter = std::unordered_map<std::string, TeamInfo>::iterator();
	std::unordered_map<std::string, int> ObjectLayer::s_WormTurnIndex = std::unordered_map<std::string, int>();

	std::vector<Gear::Ref<ExplosionEffect>> ObjectLayer::s_Explosion = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionSmokeBunddle>> ObjectLayer::s_ExplosionSmoke = std::vector<Gear::Ref<ExplosionSmokeBunddle>>();

	int ObjectLayer::s_curWorm = 0;
	int ObjectLayer::s_CurrentActivatedWormID = -1;
	bool ObjectLayer::s_turnChanged = false;

	ObjectLayer::ObjectLayer(const InitiateData& initData)
		: Layer("ObjectLayer")
	{
		for (int i = 0; i < initData.Teams.size(); ++i)
		{
			std::vector<Gear::Ref<Worm>> worms;

			s_TeamInfo.insert({ initData.Teams[i].TeamName, initData.Teams[i] });
			worms.resize(initData.Teams[i].worms.size());

			for (int j = 0; j < initData.Teams[i].worms.size(); ++j)
			{
				worms[j].reset(new Worm(i, j, initData));
			}
			s_Worms.insert({ initData.Teams[i].TeamName , worms });
			s_WormTurnIndex.insert({ initData.Teams[i].TeamName, 0 });
		}
		s_TeamIter = s_TeamInfo.begin();

		EffectPool::Init();
		ItemPool::init(initData);

		m_Transceiver = Gear::EntitySystem::CreateEntity(true);

		Gear::EventSystem::SubscribeChannel(m_Transceiver, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_Transceiver, EventChannel::World, Gear::CreateRef<ObjLayerTransceiver>());
	}

	void ObjectLayer::OnAttach()
	{
	}

	void ObjectLayer::OnDetach()
	{
		s_Worms.clear();
		s_TeamInfo.clear();
		s_CurrentActivatedWormID = -1;
		s_WormTurnIndex.clear();
	}

	void ObjectLayer::OnUpdate(Gear::Timestep ts)
	{
		if (s_turnChanged)
		{
			HandleTurnChange();
		}
		if (s_Explosion.size())
		{
			for (auto explosion = s_Explosion.begin(); explosion != s_Explosion.end(); )
			{
				(*explosion)->Update(ts);
				if (!(*explosion)->m_OnUsing)
				{
					explosion = s_Explosion.erase(explosion);
				}
				else
				{
					++explosion;
				}
			}
			for (auto& explosion : s_Explosion)
			{
				explosion->Render();
			}
		}

		if (s_ExplosionSmoke.size())
		{
			for (auto smoke = s_ExplosionSmoke.begin(); smoke != s_ExplosionSmoke.end(); )
			{
				(*smoke)->Update(ts);
				if (!(*smoke)->m_OnUsing)
				{
					smoke = s_ExplosionSmoke.erase(smoke);
				}
				else
				{
					++smoke;
				}
			}
			for (auto& smoke : s_ExplosionSmoke)
			{
				smoke->Render();
			}
		}

	}

	void ObjectLayer::OnImGuiRender()
	{
	}

	void ObjectLayer::OnEvent(Gear::Event & e)
	{
	}

	void ObjectLayer::HandleTurnChange()
	{
		s_turnChanged = false;
	
		Gear::EntitySystem::ActivateComponent(s_CurrentActivatedWormID, { {Gear::ComponentID::Controller} });

		auto FSM = Gear::EntitySystem::GetFSM(s_CurrentActivatedWormID);
		auto status = Gear::EntitySystem::GetStatus(s_CurrentActivatedWormID);
		status->SetNeedHandleData(WormStatusHandleType::Display, true);
		status->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, false);
		FSM->SetCurrentState(WormState::OnWaiting);


		std::string curWormName = s_TeamInfo[s_TeamIter->first].worms[s_curWorm].Name;
		std::pair<std::string, std::string> ChangedWormData = { s_TeamIter->first, curWormName };

		Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, ChangedWormData, s_CurrentActivatedWormID)));
	}

	void ObjectLayer::ChangeWorm()
	{
		static std::unordered_map<std::string, TeamInfo>::iterator prevTeamIter = s_TeamInfo.begin();

		if (s_CurrentActivatedWormID != -1)
		{
			Gear::EntitySystem::GetFSM(s_CurrentActivatedWormID)->SetCurrentState(WormState::OnNotMyTurn);
		}
		while (1)
		{
			s_TeamIter++;
			if (s_TeamIter == s_TeamInfo.end())
			{
				s_TeamIter = s_TeamInfo.begin();
			}
			/*if (prevTeamIter == s_TeamIter)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::GameEnd)));
				return;
			}*/
			if (s_TeamIter->second.TotalWormHp != 0)
			{
				break;
			}
		}
		while (1)
		{
			s_curWorm = ++s_WormTurnIndex[s_TeamIter->first];
			if (s_curWorm >= s_Worms[s_TeamIter->first].size())
			{
				s_WormTurnIndex[s_TeamIter->first] = 0;
				s_curWorm = 0;
			}
			s_CurrentActivatedWormID = s_Worms[s_TeamIter->first][s_curWorm]->GetID();
			int hp = std::any_cast<int>(Gear::EntitySystem::GetStatus(s_CurrentActivatedWormID)->GetStat(WormInfo::Hp));
			if (hp != 0 && Gear::EntitySystem::IsEntityActivated(s_CurrentActivatedWormID))
			{
				break;
			}
		}
		s_turnChanged = true;
	}

}

