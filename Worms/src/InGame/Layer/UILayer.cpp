#include "wmpch.h"
#include "UILayer.h"

namespace InGame {

	UILayer::UILayer(const InitiateData& initData)
		: Gear::Layer("UILayer")
	{
		m_Camera.reset(new Camera(initData));
		m_Mouse.reset(new Mouse(initData));
		m_World.reset(new World(initData));
		m_Timer.reset(new Timer(initData));
		m_WindChecker.reset(new WindChecker(initData));
		m_TurnChecker.reset(new TurnChecker(initData));
		m_ItemSelector.reset(new ItemSelector(initData));
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
		Gear::RenderCommand::SetClearColor(m_ClearColor);
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(m_Camera->m_CameraController->GetCamera());
		m_TurnChecker->Render();
		m_WindChecker->Render(ts);
	}

	void UILayer::OnImGuiRender()
	{

	}

	void UILayer::OnEvent(Gear::Event & e)
	{
		m_Camera->OnEvent(e);
	}

}