#pragma once

#include "InGame/Layer/BackGroundLayer.h"
#include "InGame/Layer/ObjectLayer.h"
#include "InGame/Layer/UILayer.h"

namespace InGame {

	class InGameScene : public Gear::Scene
	{
	public:
		InGameScene(const InitiateData& initData)
			: Gear::Scene("InGame")
		{
			//Create Event Channel
			Gear::EventSystem::CreateChannel(EventChannel::World);
			Gear::EventSystem::CreateChannel(EventChannel::Explosion);
			Gear::EventSystem::CreateChannel(EventChannel::Worm);
			Gear::EventSystem::CreateChannel(EventChannel::MouseMove);
			Gear::EventSystem::CreateChannel(EventChannel::MouseClick);
			Gear::EventSystem::CreateChannel(EventChannel::Timer);

			PushLayer(new UILayer(initData));
			PushLayer(new BackGroundLayer(initData));
			PushLayer(new ObjectLayer(initData));
		}

		inline virtual void Update(Gear::Timestep ts) override
		{
			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(ts);
			}
		}
	};

}
