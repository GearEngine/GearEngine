#include "InGameLayer.h"

#include "imgui/imgui.h"
InGameLayer::InGameLayer()
	: Layer("InGameLayer"), m_CameraController(1280.0f / 720.0f), worm({ 0.0f, 0.0f, 0.1f }, 0.0f, { 1.0f, 1.0f })
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

	MousePosition = { Gear::Input::GetMouseX(), 720.0f - Gear::Input::GetMouseY() };
	MouseWorldPosition = glm::inverse(m_CameraController.GetCamera().GetViewProjectionMatrix()) * glm::vec4(MousePosition, 0.0f, 1.0f);

	MouseWorldPosition.x /= 640;
	MouseWorldPosition.y /= 360;

	auto CameraPos = m_CameraController.GetCamera().GetPosition();
	MouseWorldPosition.x += CameraPos.x - 1.77777f * m_CameraController.GetZoomLevel();
	MouseWorldPosition.y += CameraPos.y - 1.0f * m_CameraController.GetZoomLevel();

	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::Renderer2D::DrawQuad(MouseWorldPosition, { 1.0, 1.0f }, {0.9f, 0.3f, 0.2f, 1.0f});
	Gear::Renderer2D::DrawQuad({0.0f, 0.0f}, { 1.0, 1.0f }, { 0.9f, 0.3f, 0.2f, 1.0f });
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
