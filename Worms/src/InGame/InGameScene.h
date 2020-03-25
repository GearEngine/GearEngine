#pragma once

#include "InGame/Layer/BackGroundLayer.h"
#include "InGame/Layer/ObjectLayer.h"
#include "InGame/Layer/UILayer.h"

namespace InGame {

	class InGameScene : public Gear::Scene
	{
	public:
		InGameScene(const std::string& name, Gear::Layer* imGuiLayer, const InitiateData& initData)
			: Gear::Scene(name)
		{
			PushOverlay(imGuiLayer);
			PushLayer(new UILayer(initData));
			PushLayer(new BackGroundLayer(initData));
			PushLayer(new ObjectLayer(initData));
		}

		inline virtual bool PassFirstDelay() override { return m_PassFistDelay; }
		inline virtual void Update(Gear::Timestep ts) override
		{
			if (!m_PassFistDelay)
			{
				m_ElapsedTime += ts;
				if (m_StartDelay < m_ElapsedTime)
				{
					m_PassFistDelay = true;
				}
				return;
			}
			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(ts);
			}
		}

	public:
		bool m_PassFistDelay = false;

	private:
		float m_StartDelay = 2.0f;
		float m_ElapsedTime = 0.0f;
	};

}
