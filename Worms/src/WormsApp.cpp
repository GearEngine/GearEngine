#include <Gear.h>
#include <Gear/Core/EntryPoint.h>

#include "EventChannel.h"
#include "InGame/InGameScene.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		//Load Texture
		Gear::TextureStorage::AddTexture2D("default", Gear::Texture2D::Create("assets/textures/Checkerboard.png"));
		Gear::TextureStorage::AddTexture2D("TerrainMask", Gear::Texture2D::Create("assets/textures/Terrain_Mask2.png"));
		Gear::TextureStorage::AddTexture2D("Terrain", Gear::Texture2D::Create("assets/textures/Terrain.png"));
		Gear::TextureStorage::AddFrameTexture2D("OnMove", Gear::FrameTexture2D::Create("assets/textures/wwalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("OnIdle", Gear::FrameTexture2D::Create("assets/textures/wbrth1.png", 1, 20));
		
		//Load Sound
		
		
		//Create Event Channel
		Gear::EventSystem::CreateChannel(EventChannel::Explosion);
		Gear::EventSystem::CreateChannel(EventChannel::WormMove);
		Gear::EventSystem::CreateChannel(EventChannel::MouseMove);
		Gear::EventSystem::CreateChannel(EventChannel::MouseClick);

		//Create Scene
		Gear::SceneManager::Get()->AddScene(new InGame::InGameScene("InGameScene", m_ImGuilayer, InGame::InitiateData()));

		//Current Scene Setting
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