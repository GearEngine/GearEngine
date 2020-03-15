#include "InGameLayer.h"

#include "imgui/imgui.h"
InGameLayer::InGameLayer()
	: Layer("InGameLayer"), m_CameraController(1280.0f / 720.0f), worm0({ 5.0f, 2.0f, 0.1f }, 0.0f, { 1.0f, 1.0f }), worm1({ 0.0f, 0.0f, 0.1f }, 0.0f, { 1.0f, 1.0f }), worm2({ 5.0f, 2.0f, 0.1f }, 0.0f, { 1.0f, 1.0f })
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

	if (Gear::Input::IsKeyPressd(GR_KEY_K))
	{
		worm1.SendMessage_("Event From worm1");
	}
	if (Gear::Input::IsKeyPressd(GR_KEY_J))
	{
		worm2.SendMessage_("Event From worm2");
	}

	Gear::Renderer2D::EndScene();
}

void InGameLayer::OnImGuiRender()
{
	ImGui::Text("OnScreen x : %f y : %f", MousePosition.x, MousePosition.y);
	ImGui::Text("OnWorld x : %f y : %f", MouseWorldPosition.x, MouseWorldPosition.y);
}

void InGameLayer::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}
