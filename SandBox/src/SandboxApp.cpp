#include <Gear.h>
#include <Gear/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class Sandbox : public Gear::Application
{
public:
	Sandbox()
	{
		Gear::Scene* scene = new Gear::Scene("SampleScene");
		scene->PushOverlay(m_ImGuilayer);
		scene->PushLayer(new Sandbox2D());
		Gear::SceneManager::Get()->AddScene(scene->GetName(), scene);

		Gear::Scene* scene2 = new Gear::Scene("SampleScene2");
		scene2->PushLayer(new Sandbox2D());
		Gear::SceneManager::Get()->AddScene(scene2->GetName(), scene2);

		Gear::SceneManager::Get()->changeScene("SampleScene");
	}

	~Sandbox()
	{
	}

};

Gear::Application* Gear::CreateApplication()
{
	return new Sandbox();
}