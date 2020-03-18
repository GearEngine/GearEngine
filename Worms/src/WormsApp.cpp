#include <Gear.h>
#include <Gear/Core/EntryPoint.h>

#include "InGameScene/Layer/TerrainLayer.h"
#include "EventChannel.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		//Load Resource
		Gear::TextureStorage::AddTexture2D("default", Gear::Texture2D::Create("assets/textures/Checkerboard.png"));
		Gear::TextureStorage::AddTexture2D("Terrain_Mask", Gear::Texture2D::Create("assets/textures/Terrain_Mask2.png"));
		Gear::TextureStorage::AddTexture2D("Terrain", Gear::Texture2D::Create("assets/textures/Terrain.png"));
		Gear::TextureStorage::AddFrameTexture2D("OnMove", Gear::FrameTexture2D::Create("assets/textures/wwalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("OnIdle", Gear::FrameTexture2D::Create("assets/textures/wbrth1.png", 1, 20));
		//Create Event Channel
		Gear::EventSystem::CreateChannel(EventChannel::Explosion);
		Gear::EventSystem::CreateChannel(EventChannel::WormMove);
		Gear::EventSystem::CreateChannel(EventChannel::MouseMove);

		Gear::Scene* scene = new Gear::Scene("InGameScene");
		scene->PushOverlay(m_ImGuilayer);
		scene->PushLayer(new TerrainLayer());
		scene->PushLayer(new InGameLayer());

		Mask::SetMask("Terrain_Mask", { 0.0f, -0.5f, 0.0f }, { 20.0f, 20.0f });
		Camera::SetCamera((float)Gear::WINDOW_WIDTH, (float)Gear::WINDOW_HEIGHT);

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