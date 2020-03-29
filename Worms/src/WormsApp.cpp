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
		Gear::TextureStorage::AddTexture2D("WindMask", Gear::Texture2D::Create("assets/textures/UI/WindMask.png"));
		Gear::TextureStorage::AddTexture2D("WormNameBorder", Gear::Texture2D::Create("assets/textures/UI/WormNameBorder.png"));
		Gear::TextureStorage::AddTexture2D("WormHpBorder", Gear::Texture2D::Create("assets/textures/UI/WormHpBorder.png"));
		Gear::TextureStorage::AddTexture2D("WaitingTimeBorder", Gear::Texture2D::Create("assets/textures/UI/WaitingTimeBorder.png"));

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
		std::string names[] = { "Sunwoo", "Younghwan", "TaeHwan", "Meongchorriya", "Chanho..TT", "Junsoo"};
		
		InGame::TeamInfo team1;
		InGame::TeamInfo team2;
		team1.TeamName = "Kyung";
		team1.TeamColor = InGame::TeamColor::Blue;
		team1.nWorm = 3;
		team2.TeamName = "Il";
		team2.TeamColor = InGame::TeamColor::Red;
		team2.nWorm = 3;
		initData.Teams.push_back(team1);
		initData.Teams.push_back(team2);

		for (int i = 0; i < initData.Teams.size(); ++i)
		{
			for (int j = 0; j < initData.Teams[i].nWorm; ++j)
			{
				InGame::WormSpecific worm;
				int flatIndex = i * initData.Teams.size() + j;
				worm.Name = names[i * initData.Teams.size() + j];
				worm.AdditionalZRenderOffset = flatIndex * 0.02f;
				worm.StartPosition = glm::vec3(Gear::Util::GetRndFloatFromTo(-25.0f, 25.0f), 4.0f, ZOrder::z_Worm);
				worm.Hp = initData.WormMaxHP;

				initData.Teams[i].TotalWormHp += worm.Hp;
				initData.Teams[i].worms.push_back(worm);
				initData.nTotalWorms++;
			}
		}

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