#include "ObjectLayer.h"
#include "imgui/imgui.h"

namespace InGame {

	ObjectLayer::ObjectLayer(const InitiateData& initData)
		: Layer("ObjectLayer")
	{
		worms.resize(initData.nWorm);
		worms[0].reset(new Worm(glm::vec3(1.0f, 10.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f), initData));
		worms[1].reset(new Worm(glm::vec3(2.0f, 10.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f), initData));
		worms[2].reset(new Worm(glm::vec3(3.0f, 10.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f), initData));
	}

	void ObjectLayer::OnAttach()
	{
	}

	void ObjectLayer::OnDetach()
	{
		worms.clear();
	}

	void ObjectLayer::OnUpdate(Gear::Timestep ts)
	{
		/*Gear::EntitySystem::Update(ts);
		Gear::EntitySystem::Render();*/
	}

	void ObjectLayer::OnImGuiRender()
	{
	}

	void ObjectLayer::OnEvent(Gear::Event & e)
	{
	}

}

