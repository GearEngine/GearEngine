#include "wmpch.h"
#include "MultiScene.h"

#include "Loading/LoadingScene.h"

namespace Main {

	void StartInGame()
	{
		//Temporary initData
		InGame::InitiateData initData;
		initData.Mapinfo = InGame::GetMapInfo("City");
		std::string names[] = { "Sunwoo", "Younghwan", "TaeHwan", "Meongcheriya", "Chanho..TT", "Junsoo" };

		std::vector<InGame::ItemInfo::ItemDescprition> itemList;
		std::vector<InGame::ItemInfo::ItemDescprition> itemList2;

		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Bazooka, "Bazooka", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Grenade, "Grenade", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Shotgun, "Shotgun", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Banana, "Banana", 3, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Teleport, "Teleport", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SkipGo, "Skip Go", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Surrender, "Surrender", -1, 0));

		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::PneumaticDril, "Pneumatic Dril", -1, 0));
		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::LowGravity, "Low Gravity", -1, 0));
		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::FirePunch, "Fire Punch", -1, 0));
		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Dynamite, "Dynamite", -1, 0));
		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::AirStrike, "Air Strike", -1, 0));
		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::BlowTorch, "Blow Torch", -1, 0));
		//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::NinjaRope, "Ninja Rope", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SupperBanana, "Super Banana", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::PetrolBomb, "Petrol Bomb", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::MadCow, "Mad Cow", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::HomingMissile, "Homing Missile", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::HomingPigeon, "Homing Pigeon", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SheepLauncher, "Sheep Launcher", -1, 0));
		itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Mortar, "Mortar", -1, 0));

		InGame::TeamInfo team1;
		InGame::TeamInfo team2;
		team1.TeamName = "IL";
		team1.TeamColor = InGame::TeamColor::Blue;
		team1.nWorm = 3;
		team1.TeamIcon = Gear::TextureStorage::GetTexture2D("Japan");
		team1.TeamItem = itemList;

		team2.TeamName = "KYUNG";
		team2.TeamColor = InGame::TeamColor::Red;
		team2.nWorm = 3;
		team2.TeamIcon = Gear::TextureStorage::GetTexture2D("USA");
		team2.TeamItem = itemList;


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

		if (Gear::SceneManager::Get()->isSceneExist("LoadingScene"))
		{
			Gear::SceneManager::Get()->GetScene("LoadingScene")->Init(initData);
			Gear::SceneManager::Get()->changeScene("LoadingScene");
		}
		else
		{
			auto loadingScene = new Loading::LoadingScene;
			loadingScene->Init(initData);

			Gear::SceneManager::Get()->AddScene(loadingScene);
			Gear::SceneManager::Get()->changeScene("LoadingScene");
		}
	}

}