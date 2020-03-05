#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool Sandbox2D::temp = true;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	GR_PROFILE_FUNCTION();

	m_CheckerboardTexture = Gear::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Cherno = Gear::Texture2D::Create("assets/textures/ChernoLogo.png");
}

void Sandbox2D::OnDetach()
{
	GR_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Gear::Timestep ts)
{
	GR_PROFILE_FUNCTION();
	// Update
	m_CameraController.OnUpdate(ts);

	if(Gear::Input::IsKeyPressd(GR_KEY_P))
	{
		if (temp) {
			Gear::SceneManager::Get()->changeScene("SampleScene2");
			temp = false;
		}
		else
		{
			Gear::SceneManager::Get()->changeScene("SampleScene");
			temp = true;
		}
	}

	// Render
	{
		GR_PROFILE_SCOPE("Renderer Prep");
		Gear::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gear::RenderCommand::Clear();
	}

	{
		GR_PROFILE_SCOPE("Renderer Draw");
		Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Gear::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(m_SquareRotate), m_SquareColor);
		Gear::Renderer2D::DrawQuad({ m_ChernoPosition[0], m_ChernoPosition[1] }, { 0.5f, 0.75f }, m_Cherno, 1.0f, tintColor);
		Gear::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.f, 10.0f }, glm::radians(45.0f), m_CheckerboardTexture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		Gear::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	GR_PROFILE_FUNCTION();

	ImGui::Begin("Settings1");
	ImGui::ColorEdit4("Sqaure1 Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Sqaure2 Color", glm::value_ptr(m_SquareColor2));
	ImGui::ColorEdit4("cherno tintColor Color", glm::value_ptr(tintColor));
	ImGui::SliderFloat("rotate Square1", &m_SquareRotate, -360.0f, 360.0f);
	ImGui::SliderFloat2("position", m_ChernoPosition, -10.0f, 10.f, "%.3f");
	ImGui::End();
}

void Sandbox2D::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}
