#include "wmpch.h"
#include <Gear/Core/EntryPoint.h>

#include "InGame/InGameScene.h"

class Worms : public Gear::Application
{
public:
	Worms()
	{
		//Init Font
		Font::Init();

		//Load Texture
		Gear::TextureStorage::AddTexture2D("Grad0", Gear::Texture2D::Create("assets/textures/BackGround/Grad0.png"));
		Gear::TextureStorage::AddTexture2D("default", Gear::Texture2D::Create("assets/textures/Checkerboard.png"));
		Gear::TextureStorage::AddTexture2D("TerrainMask", Gear::Texture2D::Create("assets/textures/TerrainMask.png"));
		Gear::TextureStorage::AddTexture2D("Terrain", Gear::Texture2D::Create("assets/textures/Terrain.png"));
		Gear::TextureStorage::AddTexture2D("City", Gear::Texture2D::Create("assets/textures/Map/City.png"));
		Gear::TextureStorage::AddTexture2D("CityMask", Gear::Texture2D::Create("assets/textures/Map/CityMask.png"));
		Gear::TextureStorage::AddTexture2D("BlueMountainBack", Gear::Texture2D::Create("assets/textures/Map/BlueMountainBack.png"));
		Gear::TextureStorage::AddTexture2D("TimerBorder", Gear::Texture2D::Create("assets/textures/UI/TimerBorder.png"));
		Gear::TextureStorage::AddTexture2D("WindCheckerBorder", Gear::Texture2D::Create("assets/textures/UI/WindCheckerBorder.png"));
		Gear::TextureStorage::AddTexture2D("TurnCheckerBorder", Gear::Texture2D::Create("assets/textures/UI/TurnCheckerBorder.png"));
		Gear::TextureStorage::AddTexture2D("WindMaskLeft", Gear::Texture2D::Create("assets/textures/UI/WindMaskLeft.png"));
		Gear::TextureStorage::AddTexture2D("WindMask", Gear::Texture2D::Create("assets/textures/UI/WindMask.png"));

		Gear::TextureStorage::AddFrameTexture2D("BlueWater", Gear::FrameTexture2D::Create("assets/textures/BackGround/BlueWaterWave.png", 1, 12));
		Gear::TextureStorage::AddFrameTexture2D("Cursor", Gear::FrameTexture2D::Create("assets/textures/Cursor.png", 5, 1));
		Gear::TextureStorage::AddFrameTexture2D("OnMove", Gear::FrameTexture2D::Create("assets/textures/wwalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("OnIdle", Gear::FrameTexture2D::Create("assets/textures/wbrth1.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("FallenLeaf", Gear::FrameTexture2D::Create("assets/textures/BackGround/FallenLeaf.png", 1, 128));
		Gear::TextureStorage::AddFrameTexture2D("CloudLarge", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudLarge.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("CloudMiddle", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudMiddle.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("CloudSmall", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudSmall.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("WindLeft", Gear::FrameTexture2D::Create("assets/textures/UI/WindLeft.png", 1, 8));
		Gear::TextureStorage::AddFrameTexture2D("WindRight", Gear::FrameTexture2D::Create("assets/textures/UI/WindRight.png", 1, 8));

		//Load Sound
		
		//Create Event Channel
		Gear::EventSystem::CreateChannel(EventChannel::World);
		Gear::EventSystem::CreateChannel(EventChannel::Explosion);
		Gear::EventSystem::CreateChannel(EventChannel::Worm);
		Gear::EventSystem::CreateChannel(EventChannel::MouseMove);
		Gear::EventSystem::CreateChannel(EventChannel::MouseClick);
		Gear::EventSystem::CreateChannel(EventChannel::Timer);

		//Temporary initData
		InGame::InitiateData initData;
		initData.Mapinfo = InGame::GetMapInfo("City");

		//Create InGame Scene
		Gear::SceneManager::Get()->AddScene(new InGame::InGameScene("InGameScene", m_ImGuilayer, initData));

		//Current Scene Setting
		Gear::SceneManager::Get()->changeScene("InGameScene");
	}

	~Worms()
	{
		Font::ShoutDown();
	}
};

Gear::Application* Gear::CreateApplication()
{
	return new Worms();
}

//Create Specify Order
//Create Entity
//Attach Component
//Set Component specific
//Subscpribe EventChannel
//Register Event Handler