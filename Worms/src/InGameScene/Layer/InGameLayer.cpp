#include "InGameLayer.h"

#include "imgui/imgui.h"
#include "EventChannel.h"
#include "TerrainLayer.h"

InGameLayer::InGameLayer()
	: Layer("InGameLayer"),
	worm0({ 5.0f, 2.0f, 0.1f }, 0.0f, { 1.0f, 1.0f }), 
	worm1({ 0.0f, 0.0f, 0.1f }, 0.0f, { 1.0f, 1.0f }), 
	worm2({ -1.0f, 4.0f, 0.1f }, 0.0f, { 1.0f, 1.0f })
{
	Gear::EntitySystem::InActivateComponent(worm0.GetID(), { {Gear::ComponentID::Controller} });
	Gear::EntitySystem::InActivateComponent(worm1.GetID(), { {Gear::ComponentID::Controller}});
	
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
	Gear::Renderer2D::BeginScene(Camera::s_CameraController.GetCamera());
	Gear::EntitySystem::Update(ts);
	Gear::EntitySystem::Render();
}

void InGameLayer::OnImGuiRender()
{
}

void InGameLayer::OnEvent(Gear::Event & e)
{
}
