#pragma once
#include "Component.h"

namespace Gear {

	class Status : public Component
	{
	public:
		virtual ~Status() = default;

		struct StatHandleData
		{
			std::any data;
			bool handled = false;
		};

		class StatusHandler
		{
		public:
			virtual void Handle(StatHandleData& data, std::unordered_map<std::string, std::any>& statusList) = 0;
		};

	public:
		virtual void Update(Timestep ts) override;
		void RegisterStatus(const std::initializer_list<std::pair<const std::string, std::any>>& statlist);
		void RegisterStatusHandler(const std::initializer_list<std::pair<const EnumType, Ref<StatusHandler>>>& handlers);

		inline auto& GetStatusList() { return m_StatList; }
		inline auto& GetStat(const std::string& stat) { return m_StatList[stat]; }
		void SetStat(const std::string& stat, const std::any& data);

		inline void PushNeedHandleData(EnumType type, const StatHandleData& data) { m_NeedHandleList.push_back({ type, data }); }

	private:
		std::unordered_map<std::string, std::any> m_StatList;
		std::vector<std::pair<EnumType, StatHandleData>> m_NeedHandleList;
		std::unordered_map<EnumType, Ref<StatusHandler>> m_StatusHandlers;
	};

}