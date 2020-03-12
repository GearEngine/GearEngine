#include <Gear.h>
#include <Gear/Core/EntryPoint.h>

#include "InGameScene/Layer/InGameLayer.h"
#include "box2d/box2d.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		Gear::Scene* scene = new Gear::Scene("InGameScene");
		scene->PushOverlay(m_ImGuilayer);
		scene->PushLayer(new InGameLayer());

		Gear::SceneManager::Get()->AddScene(scene->GetName(), scene);
		Gear::SceneManager::Get()->changeScene("InGameScene");
	}

	~Worms()
	{
	}
};

Gear::Application* Gear::CreateApplication()
{
	return new Worms();
}