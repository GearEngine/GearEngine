#include "grpch.h"
#include "Status.h"

namespace Gear {

	void Status::Update(Timestep ts)
	{
		if (m_NeedHandleList.size())
		{
			for (auto needHandle = m_NeedHandleList.begin(); needHandle != m_NeedHandleList.end(); )
			{
				m_StatusHandlers[needHandle->first]->Handle(needHandle->second, m_StatList);
				if (needHandle->second.handled)
				{
					needHandle = m_NeedHandleList.erase(needHandle);
				}
				else
				{
					needHandle++;
				}
			}
		}
	}

	void Status::RegisterStatus(const std::initializer_list<std::pair<const std::string, std::any>>& statlist)
	{
		m_StatList = statlist;
	}

	void Status::RegisterStatusHandler(const std::initializer_list<std::pair<const EnumType, Ref<StatusHandler>>>& handlers)
	{
		m_StatusHandlers = handlers;
	}

	void Status::SetStat(const std::string& stat, const std::any& data) 
	{
		if (m_StatList.find(stat) == m_StatList.end())
		{
			GR_CORE_WARN("The Entity has no {0} stat", stat);
			return;
		}
		m_StatList[stat] = data;
	}

}