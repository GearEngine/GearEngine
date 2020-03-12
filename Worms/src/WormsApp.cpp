#include <Gear.h>
#include <Gear/Core/EntryPoint.h>

#include "InGameScene/Layer/InGameLayer.h"
#include "InGameScene/Layer/PhysicsTestLayer.h"
#include "box2d/box2d.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		Gear::Scene* scene = new Gear::Scene("PhysicsTestLayer");
		scene->PushOverlay(m_ImGuilayer);
		scene->PushLayer(new PhysicsTestLayer());

		Gear::SceneManager::Get()->AddScene(scene->GetName(), scene);
		Gear::SceneManager::Get()->changeScene("PhysicsTestLayer");
	}

	~Worms()
	{
	}
};

Gear::Application* Gear::CreateApplication()
{
	return new Worms();
}