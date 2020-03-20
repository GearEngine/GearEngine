#pragma once

#include "InGame/Entity/UI/Camera/Camera.h"
#include "InGame/Entity/UI/Mouse/Mouse.h"

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
		Gear::Ref<Camera> m_Camera;
		Gear::Ref<Mouse> m_Mouse;
		glm::vec4 m_ClearColor = { 0.14117, 0.16470, 0.29019, 1.0 };
	};

}