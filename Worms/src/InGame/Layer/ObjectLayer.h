#pragma once

#include "InGame/Entity/Object/Worm/Worm.h"
#include "InGame/Data/InitiateData.h"

namespace InGame {

	class ObjectLayer : public Gear::Layer
	{
	public:
		ObjectLayer(const InitiateData& initData);
		virtual ~ObjectLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Gear::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Gear::Event& e) override;

	public:
		static void ChangeWorm();

	private:
		std::vector<Gear::Ref<Worm>> m_Worms;
		static int m_CurrentActiveWorm;
		static int m_nWorms;
		static bool m_turnChanged;

		int m_Transceiver; // Entity
	};

	class Transceiver : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewStart)
			{
				ObjectLayer::ChangeWorm();
			}
		}
	};

}
