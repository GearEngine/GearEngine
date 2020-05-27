#pragma once

#include "InGame/Entity/UI/Camera/Camera.h"
#include "InGame/Entity/UI/Mouse/Mouse.h"
#include "InGame/Entity/World/World.h"
#include "InGame/Entity/UI/Checker/Timer.h"
#include "InGame/Entity/UI/Checker/WindChecker.h"
#include "InGame/Entity/UI/Checker/TurnChecker.h"
#include "InGame/Entity/UI/ItemSelector/ItemSelector.h"
#include "InGame/Entity/UI/WindowSelector/WindowSelector.h"

namespace InGame {

	class UILayer : public Gear::Layer 
	{
	public:
		UILayer(const InitiateData& initData);
		virtual ~UILayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Gear::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Gear::Event& e) override;

	private:
		bool escape = false;

		Gear::Ref<Camera> m_Camera;
		Gear::Ref<Mouse> m_Mouse;
		Gear::Ref<World> m_World;
		Gear::Ref<Timer> m_Timer;
		Gear::Ref<WindChecker> m_WindChecker;
		Gear::Ref<TurnChecker> m_TurnChecker;
		Gear::Ref<ItemSelector> m_ItemSelector;
		Gear::Ref<WindowSelector> m_WindowSelector;

		glm::vec4 m_ClearColor = { 0.14117, 0.16470, 0.29019, 1.0 };
	};

}