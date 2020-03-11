#include "InGameLayer.h"

#include "imgui/imgui.h"
InGameLayer::InGameLayer()
	: Layer("InGameLayer"), m_CameraController(1280.0f / 720.0f), worm(glm::vec3(0.0f, 0.0f, 0.1f))
{
}

void InGameLayer::OnAttach()
{
	//AddSound("assets/Sound/show only me.mp3", "bol4", true, true);
	//PlaySound(GetSound("bol4"), SoundChannel::BGM);
}

void InGameLayer::OnDetach()
{
}

void InGameLayer::OnUpdate(Gear::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	
	Gear::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Gear::RenderCommand::Clear();
	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::Renderer2D::DrawQuad( { 0.0f, -4.0f, -0.1f }, { 5.0f, 5.0f }, {0.6f, 0.3f, 0.5f, 0.1f});
	Gear::Renderer2D::EndScene();
}

void InGameLayer::OnImGuiRender()
{
}

void InGameLayer::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}
