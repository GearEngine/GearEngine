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
	};

}
