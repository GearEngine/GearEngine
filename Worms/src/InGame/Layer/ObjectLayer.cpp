#include "ObjectLayer.h"
#include "imgui/imgui.h"

namespace InGame {

	ObjectLayer::ObjectLayer(const InitiateData& initData)
		: Layer("ObjectLayer")
	{

	}

	void ObjectLayer::OnAttach()
	{
	}

	void ObjectLayer::OnDetach()
	{
	}

	void ObjectLayer::OnUpdate(Gear::Timestep ts)
	{
		Gear::EntitySystem::Update(ts);
		Gear::EntitySystem::Render();
	}

	void ObjectLayer::OnImGuiRender()
	{
	}

	void ObjectLayer::OnEvent(Gear::Event & e)
	{
	}

}

