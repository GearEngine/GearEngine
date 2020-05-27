#include "grpch.h"
#include "EntitySystem.h"

namespace Gear {

	int EventSystem::s_channelID = 0;
	std::unordered_map<int, Ref<EventChannel>> EventSystem::s_Channels = std::unordered_map<int, Ref<EventChannel>>();
	std::unordered_map<int, std::set<ChannelType>> EventSystem::s_IgnoreChannel = std::unordered_map<int, std::set<ChannelType>>();
	std::set<int> EventSystem::s_IgnoredEventEntity = std::set<int>();

	EventChannel::EventChannel(ChannelType channel, int id)
		: m_Channel(channel), m_ChannelID(id)
	{
	}

	void EventChannel::Subscribe(int entityID)
	{
		for (auto &e : m_Subscriber)
		{
			if (e->m_EntityID == entityID)
			{
				GR_CORE_WARN("{0}entity aleady subscribe {1} channel", entityID, m_Channel);
			}
		}
		m_Subscriber.push_back(EntitySystem::GetEntity(entityID));
		DEBUG_GR_CORE_TRACE("On {0} {1}entity subscribe channel", m_Channel, entityID);
	}

	void EventChannel::UnSubscribe(int entityID)
	{
		for (auto it = m_Subscriber.begin(); it != m_Subscriber.end(); ++it)
		{
			if ((*it)->m_EntityID == entityID)
			{
				m_Subscriber.erase(it);
				DEBUG_GR_CORE_TRACE("On {0} : {1}entity unsubscribe channel", m_Channel, entityID);
				return;
			}
		}
		GR_CORE_WARN("{0} entity doesn't subscribe {1} channel", entityID, m_Channel);
	}

	void EventChannel::DispatchEvent(const EntityEvent& event)
	{
		for (auto& entity : m_Subscriber)
		{
			if (CheckIgnore(m_Channel, entity->m_EntityID)) continue;
			if (entity->m_OnActivate)
			{
				entity->m_EventQueue.push_back(event);
			}
		}
	}

	void EventChannel::DispatchEventOnce(const EntityEvent& event)
	{
		auto type = event.Type;
		for (auto& entity : m_Subscriber)
		{
			if (CheckIgnore(m_Channel, entity->m_EntityID)) continue;

			bool alreadyExist = false;
			for (auto event : entity->m_EventQueue)
			{
				if (event.Type == type)
				{
					alreadyExist = true;
					break;
				}
			}
			if (!alreadyExist)
			{
				entity->m_EventQueue.push_back(event);
			}
		}
	}

	void EventChannel::DispatchEventTo(const EntityEvent & event, int entityID)
	{
		for (auto& entity : m_Subscriber)
		{
			if (entity->m_OnActivate)
			{
				if (entity->m_EntityID == entityID)
				{
					entity->m_EventQueue.push_back(event);
				}
			}
		}
	}

	void EventChannel::DispatchEventOnceTo(const EntityEvent& event, int entityID)
	{
		for (auto& entity : m_Subscriber)
		{
			if (entity->m_OnActivate)
			{
				if (entity->m_EntityID == entityID)
				{
					bool alreadyExist = false;
					auto type = event.Type;
					for (auto event : entity->m_EventQueue)
					{
						if (event.Type == type)
						{
							alreadyExist = true;
							break;
						}
					}
					if (!alreadyExist)
					{
						entity->m_EventQueue.push_back(event);
					}
					break;
				}
			}
		}
	}

	void EventSystem::Reset()
	{
		s_channelID = 0;
		s_Channels.clear();
		s_IgnoreChannel.clear();
		s_IgnoredEventEntity.clear();
	}

	void EventSystem::Shutdown()
	{
		Reset();
	}



	void EventSystem::CreateChannel(ChannelType channel)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel != s_Channels.end())
		{
			GR_CORE_WARN("{0} channel alredy exist!", channel);
			return;
		}
		s_Channels.insert({ s_channelID, CreateRef<EventChannel>(channel, s_channelID) });
		s_channelID++;
		DEBUG_GR_CORE_TRACE("{0} channel created!", channel);
	}

	void EventSystem::SubscribeChannel(int entityID, ChannelType channel)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel == s_Channels.end())
		{
			GR_CORE_TRACE("{0} channel doesn't exist", channel);
			return;
		}
		findChannel->second->Subscribe(entityID);
	}

	void EventSystem::UnSubscribeChannel(int entityID, ChannelType channel)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel == s_Channels.end())
		{
			GR_CORE_WARN("{0} channel doesn't exist", channel);
			return;
		}
		findChannel->second->UnSubscribe(entityID);
	}

	void EventSystem::DispatchEvent(ChannelType channel, const EntityEvent& event)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel == s_Channels.end())
		{
			GR_CORE_WARN("On Entity Event Dispatch : {0} channel doesn't exist", channel);
			return;
		}
		findChannel->second->DispatchEvent(event);
	}

	void EventSystem::RegisterEventHandler(int entityID, ChannelType channel, Ref<EventHandler> handler)
	{
		auto entity = EntitySystem::m_EntityPool.find(entityID);
		if (entity == EntitySystem::m_EntityPool.end())
		{
			GR_CORE_WARN("{0} entity doesn't exist!", entityID);
			return;
		}
		entity->second->m_EventHandler[channel] = handler;
	}
	void EventSystem::DispatchEventOnce(ChannelType channel, const EntityEvent & event)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel == s_Channels.end())
		{
			GR_CORE_WARN("On Entity Event Dispatch : {0} channel doesn't exist", channel);
			return;
		}
		findChannel->second->DispatchEventOnce(event);
	}

	void EventSystem::DispatchEventTo(ChannelType channel, const EntityEvent & event, int entityID)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel == s_Channels.end())
		{
			GR_CORE_WARN("On Entity Event Dispatch : {0} channel doesn't exist", channel);
			return;
		}
		if (CheckIgnore(channel, entityID) == false)
		{
			findChannel->second->DispatchEventTo(event, entityID);
		}
	}

	void EventSystem::DispatchEventOnceTo(ChannelType channel, const EntityEvent & event, int entityID)
	{
		auto findChannel = s_Channels.find(channel);
		if (findChannel == s_Channels.end())
		{
			GR_CORE_WARN("On Entity Event Dispatch : {0} channel doesn't exist", channel);
			return;
		}
		if (CheckIgnore(channel, entityID) == false)
		{
			findChannel->second->DispatchEventOnceTo(event, entityID);
		}
	}

	void EventSystem::IgnoreAllEvent(int entityID)
	{
		for (auto& channel : s_Channels)
		{
			s_IgnoreChannel[entityID].insert(channel.first);
		}
		s_IgnoredEventEntity.insert(entityID);
	}

	void EventSystem::IgnoreChannelEvent(ChannelType channel, int entityID)
	{
		s_IgnoreChannel[entityID].erase(channel);
		if (s_IgnoreChannel[entityID].empty())
		{
			s_IgnoredEventEntity.erase(entityID);
		}
	}

	void EventSystem::ReAcceptAllEvent(int entityID)
	{
		auto findIgnoreChannel = s_IgnoreChannel.find(entityID);
		if (findIgnoreChannel != s_IgnoreChannel.end())
		{
			s_IgnoreChannel.erase(findIgnoreChannel);
			s_IgnoredEventEntity.erase(entityID);
		}
	}

	void EventSystem::ReAcceptChannelEvent(ChannelType channel, int entityID)
	{
		auto findIgnoreChannel = s_IgnoreChannel.find(entityID);
		if (findIgnoreChannel != s_IgnoreChannel.end())
		{
			findIgnoreChannel->second.erase(channel);
			if (findIgnoreChannel->second.empty())
			{
				s_IgnoredEventEntity.erase(entityID);
			}
		}
	}

	bool CheckIgnore(ChannelType channel, int entityID)
	{
		if (EventSystem::s_IgnoreChannel.empty())
		{
			return false;
		}

		auto findIgnoreChannel = EventSystem::s_IgnoreChannel.find(entityID);
		if (findIgnoreChannel == EventSystem::s_IgnoreChannel.end())
		{
			return false;
		}

		if (findIgnoreChannel->second.find(channel) != findIgnoreChannel->second.end())
		{
			return true;
		}
		return false;
	}
}
