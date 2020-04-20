#include "grpch.h"
#include "Status.h"

namespace Gear {

	void Status::Update(Timestep ts)
	{
		if (m_RegistedPushedList.size())
		{
			for (auto& needHandle : m_RegistedPushedList)
			{
				m_NeedHandleList.push_back(needHandle);
			}
			m_RegistedPushedList.clear();
		}


		if (m_NeedHandleList.size())
		{
			for (auto needHandle = m_NeedHandleList.begin(); needHandle != m_NeedHandleList.end(); )
			{
				if (needHandle->second.Handled)
				{
					needHandle = m_NeedHandleList.erase(needHandle);
					continue;
				}

				if (needHandle->second.Paused)
				{
					needHandle++;
					continue;
				}
				else
				{
					m_StatusHandlers[needHandle->first]->Handle(m_ID, needHandle->second, m_StatList);
					needHandle++;
				}
			}
		}
	}

	void Status::PopNeedHandleData(EnumType type)
	{
		if (m_NeedHandleList.size())
		{
			for (auto needHandle = m_NeedHandleList.begin(); needHandle != m_NeedHandleList.end(); needHandle++)
			{
				if (needHandle->first == type)
				{
					m_StatusHandlers[type]->OnOut();
					m_NeedHandleList.erase(needHandle);
					return;
				}
			}
		}
	}

	void Status::SetNeedHandleData(EnumType type, bool pause, bool handled)
	{
		if (m_NeedHandleList.size())
		{
			for (auto needHandle = m_NeedHandleList.begin(); needHandle != m_NeedHandleList.end(); needHandle++)
			{
				if (needHandle->first == type)
				{
					needHandle->second.Paused = pause;
					needHandle->second.Handled = handled;
					return;
				}
			}
		}
	}


	void Status::RegisterStatus(const std::initializer_list<std::pair<const EnumType, std::any>>& statlist)
	{
		m_StatList = statlist;
	}

	void Status::RegisterStatusHandler(const std::initializer_list<std::pair<const EnumType, Ref<StatusHandler>>>& handlers)
	{
		m_StatusHandlers = handlers;
	}

	void Status::SetStat(EnumType stat, const std::any& data)
	{
		if (m_StatList.find(stat) == m_StatList.end())
		{
			GR_CORE_WARN("The Entity has no {0} stat", stat);
			return;
		}
		m_StatList[stat] = data;
	}

}