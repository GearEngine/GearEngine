#include <Gear.h>
#include <Gear/Core/EntryPoint.h>

#include "InGameScene/Layer/InGameLayer.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		//Load Resource
		Gear::TextureStorage::AddFrameTexture2D("OnMove", Gear::FrameTexture2D::Create("assets/textures/wwalk.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("OnIdle", Gear::FrameTexture2D::Create("assets/textures/wbrth1.png", 1, 10));

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