#pragma once
#include "Entity.h"

namespace Gear {

	using ChannelType = unsigned int;
	

	class EventHandler
	{
	public:
		virtual void Handle(std::any data, int entityID, bool& handled) = 0;
	};

	class EventChannel
	{
	public:
		EventChannel(ChannelType channel, int id);

	public:
		void Subscribe(int entityID);
		void UnSubscribe(int entityID);

		void DispatchEvent(const EntityEvent& event);
		void DispatchEventOnce(const EntityEvent& event);
		void DispatchEventTo(const EntityEvent& event, int entityID);
		void DispatchEventOnceTo(const EntityEvent& event, int entityID);

	private:
		ChannelType m_Channel;
		int m_ChannelID;
		std::vector<Ref<Entity>> m_Subscriber;
	};

	class EventSystem
	{
	private:
		static void Reset();
		static void Shutdown();

	public:
		static void CreateChannel(ChannelType channel);
		static void SubscribeChannel(int entityID, ChannelType channel);
		static void UnSubscribeChannel(int entityID, ChannelType channel);
		static void DispatchEvent(ChannelType channel, const EntityEvent& event);
		static void RegisterEventHandler(int entityID, ChannelType channel, Ref<EventHandler> handler);
		static void DispatchEventOnce(ChannelType channel, const EntityEvent& event);
		static void DispatchEventTo(ChannelType channel, const EntityEvent& event, int entityID);
		static void DispatchEventOnceTo(ChannelType channel, const EntityEvent& event, int entityID);

	private:
		static int s_channelID;
		static std::unordered_map<int, Ref<EventChannel>> s_Channels;
		friend class EntitySystem;
	};
}