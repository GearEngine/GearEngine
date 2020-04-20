#pragma once
#include "Component.h"

namespace Gear {

	class Status : public Component
	{
	public:
		Status() = default;
		Status(int id)
			: Component(id)
		{}

		virtual ~Status() = default;

		struct StatHandleData
		{
			StatHandleData(const std::any& data, bool paused = false)
				: Data(data), Paused(paused)
			{}
			std::any Data;
			bool Handled = false;
			bool Paused;
		};

		class StatusHandler
		{
		public:
			virtual void OnOut() {}
			virtual void Handle(int entityID, StatHandleData& data, std::unordered_map<EnumType, std::any>& statusList) = 0;
		};

	public:
		virtual void Update(Timestep ts) override;
		void RegisterStatus(const std::initializer_list<std::pair<const EnumType, std::any>>& statlist);
		void RegisterStatusHandler(const std::initializer_list<std::pair<const EnumType, Ref<StatusHandler>>>& handlers);

		inline auto& GetStatusList() { return m_StatList; }
		inline auto& GetStat(EnumType stat) { return m_StatList[stat]; }
		void SetStat(EnumType stat, const std::any& data);

		void PopNeedHandleData(EnumType type);
		void SetNeedHandleData(EnumType type, bool pause, bool handled = false);

		inline void PushNeedHandleData(EnumType type, const StatHandleData& data) 
		{
			m_NeedHandleList.push_back({ type, data });
		}
		inline void RegisterPushNeedHandleData(EnumType type, const StatHandleData& data)
		{
			m_RegistedPushedList.push_back({ type, data });
		}

	private:
		std::unordered_map<EnumType, std::any> m_StatList;
		std::vector<std::pair<EnumType, StatHandleData>> m_NeedHandleList;
		std::unordered_map<EnumType, Ref<StatusHandler>> m_StatusHandlers;

		std::vector<std::pair<EnumType, StatHandleData>> m_RegistedPushedList;
	};

}