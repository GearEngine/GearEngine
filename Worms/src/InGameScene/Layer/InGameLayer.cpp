#include "InGameLayer.h"

#include "imgui/imgui.h"
#include "EventChannel.h"

InGameLayer::InGameLayer()
	: Layer("InGameLayer"), m_CameraController(1280.0f / 720.0f), 
	worm0({ 5.0f, 2.0f, 0.1f }, 0.0f, { 1.0f, 1.0f }), 
	worm1({ 0.0f, 0.0f, 0.1f }, 0.0f, { 1.0f, 1.0f }), 
	worm2({ -1.0f, 4.0f, 0.1f }, 0.0f, { 1.0f, 1.0f })
{
	Gear::EntitySystem::InActivateComponent(worm1.GetID(), { {Gear::ComponentID::Controller} });
	Gear::EntitySystem::InActivateComponent(worm2.GetID(), { {Gear::ComponentID::Controller} });
	
	Gear::EventSystem::UnSubscribeChannel(worm1.GetID(), EventChannel::Explosion);
	Gear::EventSystem::UnSubscribeChannel(worm2.GetID(), EventChannel::Explosion);
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
	worm0.Update(ts);
	Gear::EntitySystem::Update(ts);

	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::EntitySystem::Render();
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
