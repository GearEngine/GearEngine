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
		Gear::TextureStorage::AddTexture2D("Japan", Gear::Texture2D::Create("assets/textures/UI/Japan.png"));
		Gear::TextureStorage::AddTexture2D("USA", Gear::Texture2D::Create("assets/textures/UI/USA.png"));
		Gear::TextureStorage::AddTexture2D("RedHpBar", Gear::Texture2D::Create("assets/textures/UI/RedHp.png"));
		Gear::TextureStorage::AddTexture2D("BlueHpBar", Gear::Texture2D::Create("assets/textures/UI/BlueHp.png"));
		Gear::TextureStorage::AddTexture2D("WhiteHpBar", Gear::Texture2D::Create("assets/textures/UI/WhiteHp.png"));
		
		Gear::TextureStorage::AddFrameTexture2D("BlueWater", Gear::FrameTexture2D::Create("assets/textures/BackGround/BlueWaterWave.png", 1, 12));
		Gear::TextureStorage::AddFrameTexture2D("Cursor", Gear::FrameTexture2D::Create("assets/textures/Cursor.png", 5, 1));
		Gear::TextureStorage::AddFrameTexture2D("OnUseItem", Gear::FrameTexture2D::Create("assets/textures/wairbakd.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("FallenLeaf", Gear::FrameTexture2D::Create("assets/textures/BackGround/FallenLeaf.png", 1, 128));
		Gear::TextureStorage::AddFrameTexture2D("CloudLarge", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudLarge.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("CloudMiddle", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudMiddle.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("CloudSmall", Gear::FrameTexture2D::Create("assets/textures/BackGround/CloudSmall.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("WindLeft", Gear::FrameTexture2D::Create("assets/textures/UI/WindLeft.png", 1, 8));
		Gear::TextureStorage::AddFrameTexture2D("WindRight", Gear::FrameTexture2D::Create("assets/textures/UI/WindRight.png", 1, 8));
		Gear::TextureStorage::AddFrameTexture2D("RedFollowArrow", Gear::FrameTexture2D::Create("assets/textures/UI/RedFollowingArrow.png", 1, 16));
		Gear::TextureStorage::AddFrameTexture2D("BlueFollowArrow", Gear::FrameTexture2D::Create("assets/textures/UI/BlueFollowingArrow.png", 1, 16));

		//Worm
		Gear::TextureStorage::AddFrameTexture2D("Drawn", Gear::FrameTexture2D::Create("assets/textures/Worm/Drawn.png", 1, 4));
		//walk
		Gear::TextureStorage::AddFrameTexture2D("LeftFlatWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/LeftFlatWalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/LeftUpWalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/LeftDownWalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/RightFlatWalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("RightUpWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/RightUpWalk.png", 1, 15));
		Gear::TextureStorage::AddFrameTexture2D("RightDownWalk", Gear::FrameTexture2D::Create("assets/textures/Worm/Walk/RightDownWalk.png", 1, 15));
		//Breath
		Gear::TextureStorage::AddFrameTexture2D("LeftFlatBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/LeftFlatBreath.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/LeftUpBreath.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/LeftDownBreath.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/RightFlatBreath.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("RightUpBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/RightUpBreath.png", 1, 20));
		Gear::TextureStorage::AddFrameTexture2D("RightDownBreath", Gear::FrameTexture2D::Create("assets/textures/Worm/Breath/RightDownBreath.png", 1, 20));
		//Jump
		Gear::TextureStorage::AddFrameTexture2D("LeftJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftJumping.png", 1, 7));
		Gear::TextureStorage::AddFrameTexture2D("RightJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightJumping.png", 1, 7));
		Gear::TextureStorage::AddFrameTexture2D("LeftBackFlip", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftBackFlip.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("RightBackFlip", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightBackFlip.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightFlatReadyJump.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("RightUpReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightUpReadyJump.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("RightDownReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightDownReadyJump.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("LeftFlatReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftFlatReadyJump.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftUpReadyJump.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownReadyJump", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftDownReadyJump.png", 1, 10));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightFlatLand.png", 1, 6));
		Gear::TextureStorage::AddFrameTexture2D("RightUpLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightUpLand.png", 1, 6));
		Gear::TextureStorage::AddFrameTexture2D("RightDownLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/RightDownLand.png", 1, 6));
		Gear::TextureStorage::AddFrameTexture2D("LeftFlatLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftFlatLand.png", 1, 6));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftUpLand.png", 1, 6));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownLand", Gear::FrameTexture2D::Create("assets/textures/Worm/Jump/LeftDownLand.png", 1, 6));
		//Fallen
		Gear::TextureStorage::AddFrameTexture2D("LeftFlatSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatSliding.png", 1, 3));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpSliding.png", 1, 3));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownSliding.png", 1, 3));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatSliding.png", 1, 3));
		Gear::TextureStorage::AddFrameTexture2D("RightUpSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpSliding.png", 1, 3));
		Gear::TextureStorage::AddFrameTexture2D("RightDownSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownSliding.png", 1, 3));
		
		Gear::TextureStorage::AddFrameTexture2D("LeftFlatStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatStuck.png", 1, 49));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpStuck.png", 1, 49));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownStuck.png", 1, 49));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatStuck.png", 1, 49));
		Gear::TextureStorage::AddFrameTexture2D("RightUpStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpStuck.png", 1, 49));
		Gear::TextureStorage::AddFrameTexture2D("RightDownStuck", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownStuck.png", 1, 49));

		Gear::TextureStorage::AddFrameTexture2D("LeftFlatAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatAfterSliding.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpAfterSliding.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownAfterSliding.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatAfterSliding.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("RightUpAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpAfterSliding.png", 1, 22));
		Gear::TextureStorage::AddFrameTexture2D("RightDownAfterSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownAfterSliding.png", 1, 22));

		Gear::TextureStorage::AddFrameTexture2D("LeftFlatAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftFlatAfterDamagedSliding.png", 1, 36));
		Gear::TextureStorage::AddFrameTexture2D("LeftUpAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftUpAfterDamagedSliding.png", 1, 36));
		Gear::TextureStorage::AddFrameTexture2D("LeftDownAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/LeftDownAfterDamagedSliding.png", 1, 36));
		Gear::TextureStorage::AddFrameTexture2D("RightFlatAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightFlatAfterDamagedSliding.png", 1, 36));
		Gear::TextureStorage::AddFrameTexture2D("RightUpAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightUpAfterDamagedSliding.png", 1, 36));
		Gear::TextureStorage::AddFrameTexture2D("RightDownAfterDamagedSliding", Gear::FrameTexture2D::Create("assets/textures/Worm/Fallen/RightDownAfterDamagedSliding.png", 1, 36));

		//animation
		std::vector<std::pair<int, int>> orderVector;
		for (int i = 0; i < 32; ++i)
		{
			if (i < 16) 
			{
				orderVector.push_back({ 0, i });
			}
			else
			{
				orderVector.push_back({ 0, 31 - i });
			}
		}
		auto redArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RedFollowArrow"), 0.02f, orderVector, true);
		auto blueArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("BlueFollowArrow"), 0.02f, orderVector, true);
		redArrow->Start();
		blueArrow->Start();
		Gear::TextureStorage::AddAnimation("RedFollowArrow", redArrow);
		Gear::TextureStorage::AddAnimation("BlueFollowArrow", blueArrow);
		
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
		std::string names[] = { "Sunwoo", "Younghwan", "TaeHwan", "Meongcheriya", "Chanho..TT", "Junsoo"};
		
		InGame::TeamInfo team1;
		InGame::TeamInfo team2;
		team1.TeamName = "IL";
		team1.TeamColor = InGame::TeamColor::Blue;
		team1.nWorm = 3;
		team1.TeamIcon = Gear::TextureStorage::GetTexture2D("Japan");
		team2.TeamName = "KYUNG";
		team2.TeamColor = InGame::TeamColor::Red;
		team2.nWorm = 3;
		team2.TeamIcon = Gear::TextureStorage::GetTexture2D("USA");
		initData.Teams.push_back(team1);
		initData.Teams.push_back(team2);

		for (int i = 0; i < initData.Teams.size(); ++i)
		{
			for (int j = 0; j < initData.Teams[i].nWorm; ++j)
			{
				InGame::WormSpecific worm;
				int flatIndex = i * initData.Teams[i].nWorm + j;
				worm.Name = names[i * initData.Teams[i].nWorm + j];
				worm.AdditionalZRenderOffset = flatIndex * 0.02f;
				worm.StartPosition = glm::vec3(Gear::Util::GetRndFloatFromTo(initData.Mapinfo.TerrainMinX, initData.Mapinfo.TerrainMaxX), 5.0f, ZOrder::z_Worm);
				worm.Hp = initData.WormMaxHP;
				worm.Direction = (InGame::WormInfo::DirectionType)Gear::Util::GetRndInt(2);

				initData.Teams[i].TotalWormHp += worm.Hp;
				initData.Teams[i].worms.push_back(worm);
				initData.nTotalWorms++;
			}
			initData.Teams[i].CurrentTotalWormHp = initData.Teams[i].TotalWormHp;
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