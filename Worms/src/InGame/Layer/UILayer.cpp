#include "wmpch.h"
#include "UILayer.h"

namespace InGame {

	UILayer::UILayer(const InitiateData& initData)
		: Gear::Layer("UILayer")
	{
		m_Camera.reset(new Camera(initData.WindowWidth, initData.WindowHeight));
	}

	void UILayer::OnAttach()
	{
	}

	void UILayer::OnDetach()
	{
	}

	void UILayer::OnUpdate(Gear::Timestep ts)
	{
		m_Camera->OnUpdate(ts);
	}

	void UILayer::OnImGuiRender()
	{
	}

	void UILayer::OnEvent(Gear::Event & e)
	{
		m_Camera->OnEvent(e);
	}

}