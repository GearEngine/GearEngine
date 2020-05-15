#pragma once

#include "InGame/Entity/Object/Worm/Worm.h"
#include "InGame/Entity/Object/Effects/Effects.h"

namespace InGame {

	class ObjectLayer : public Gear::Layer
	{
	public:
		ObjectLayer(const InitiateData& initData);
		virtual ~ObjectLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Gear::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Gear::Event& e) override;

	private:
		void HandleTurnChange();

	public:
		static void ChangeWorm();
		static int GameEndCheck();
		static void TurnCheck();

		static std::vector<Gear::Ref<ExplosionEffect>> s_Explosion;
		static std::vector<Gear::Ref<ExplosionSmokeBunddle>> s_ExplosionSmoke;
		static std::vector<Gear::Ref<FlameBundle>> s_Flames;
		static std::list<Gear::Ref<Smoke>> s_Smokes;
		static std::list<Gear::Ref<Exhaust>> s_Exhausts;

	private:
		//for turn count
		static std::unordered_map<std::string, std::vector<Gear::Ref<Worm>>> s_Worms;
		static std::unordered_map<std::string, TeamInfo> s_TeamInfo;
		static std::unordered_map<std::string, TeamInfo>::iterator s_TeamIter;

		static std::unordered_map<std::string, int> s_WormTurnIndex;
		static bool s_turnChanged;
		static int s_CurrentActivatedWormID;
		static int s_curWorm;

		int m_Transceiver; // Entity
	};

	class ObjLayerTransceiver : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewStart)
			{
				ObjectLayer::ChangeWorm();
			}
			handled = true;
		}
	};

}
