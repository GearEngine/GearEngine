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

	//m_CheckerboardTexture = Gear::Texture2D::Create("assets/textures/Checkerboard.png");
	auto airStrike = Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10);
	/*auto idle = Gear::FrameTexture2D::Create("assets/textures/wbrth1.png", 1, 15);
	auto walk = Gear::FrameTexture2D::Create("assets/textures/wwalk.png", 1, 15);*/
	wormsAnimation = Gear::Animation2D::Create(airStrike, 0.1f, false);
	/*wormsAnimation.AddAnimation("Idle", Gear::Animation2D::Create(idle, 0.05f, false));
	wormsAnimation.AddAnimation("Walk", Gear::Animation2D::Create(walk, 0.05f, false));*/
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

	WindowWomrsPosition = glm::vec4(wormsPosition, 1.0f);

	//screen position
	WindowWomrsPosition = m_CameraController.GetCamera().GetViewProjectionMatrix() * WindowWomrsPosition;
	WindowWomrsPosition.x *= 640;
	WindowWomrsPosition.x += 640;
	WindowWomrsPosition.y *= 360;
	WindowWomrsPosition.y += 360;

	if (Gear::Input::IsKeyPressd(GR_KEY_SPACE))
	{
		wormsAnimation->Start();
	}
	wormsAnimation->Update(ts);

	/*velocity += ts * gravity * 0.01;
	if (wormsPosition.y == 0.0)
		velocity = 0.0f;
	wormsPosition.y -= velocity;*/

	auto color = Gear::Renderer2D::getPixel((int)WindowWomrsPosition.x, (int)WindowWomrsPosition.y - 30);
	auto[r, g, b] = color;
	if (r != 25 && g != 25 && b != 25) 
	{
		wormsPosition.y += velocity;
		velocity = 0;
	}



	if (Gear::Input::IsKeyPressd(GR_KEY_P))
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
		//Gear::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(m_SquareRotate), m_SquareColor);
		//Gear::Renderer2D::DrawQuad({ m_ChernoPosition[0], m_ChernoPosition[1] }, { 1.0f, 1.0f }, m_Cherno, 1.0f, tintColor);
		//Gear::Renderer2D::DrawQuad({ m_ChernoPosition[0], m_ChernoPosition[1] }, { 1.0f, 1.0f }, m_Cherno, 1.0f, tintColor);

		Gear::Renderer2D::DrawQuad({ 0.0f, -4.0f }, { 2.0f, 1.0f }, m_SquareColor);
		Gear::Renderer2D::DrawAnimationRotateQuad(wormsPosition, { 1.f, 1.0f }, glm::radians(0.0f), wormsAnimation, tintColor);
		//Gear::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 1.f, 1.0f }, glm::radians(0.0f), m_CheckerboardTexture, 1.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f), 1);
		Gear::Renderer2D::EndScene();




		//Gear::Renderer2D::EndScene(Gear::Input::GetMouseX(), Gear::Input::GetMouseY());
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
	ImGui::SliderFloat("worms Y position", &wormsPosition.y, 0, 10.0f);
	ImGui::SliderFloat("velocty", &velocity, 0, 1.0f);
	ImGui::SliderFloat2("position", m_ChernoPosition, -10.0f, 10.f, "%.3f");
	ImGui::Text("WomrsWindowPosition x : %f y : %f", WindowWomrsPosition.x, WindowWomrsPosition.y);
	ImGui::End();
}

void Sandbox2D::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}
