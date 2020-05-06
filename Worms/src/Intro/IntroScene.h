#pragma once

#include "IntroScene2.h"

namespace Intro {

	void ResourceLoading();

	class IntroScene : public Gear::Scene
	{
	private:
		std::thread loadingThread;
		glm::mat4 introTranslate;
		Gear::Ref<Gear::Texture2D> introTexture;
		Gear::OrthographicCamera introCamera;

	public:
		static bool loadingComplete;
		bool inFirst = true;
		bool isEndScene = false;
		glm::vec4 alpha;
		float pastTime = 0.0f;
		float sceneChangeDelay = 3.0f;

	public:
		IntroScene()
			: Scene("IntroScene"), introCamera(-1.7777f, 1.7777f, -1.0f, 1.0f), alpha(1.0f, 1.0f, 1.0f, 0.0f)
		{
			Gear::TextureStorage::AddTexture2D("Intro", Gear::Texture2D::Create("assets/textures/Intro/TEAM17.png"));

			loadingThread = std::thread(ResourceLoading);
			loadingThread.detach();

			introTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.6f, 2.0f, 1.0f));
			introTexture = Gear::TextureStorage::GetTexture2D("Intro");
		}

		virtual void Update(Gear::Timestep ts) override
		{

			if (loadingComplete )
			{
				//GR_INFO("Resources Loading complete");
			}

			Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			Gear::RenderCommand::Clear();

			Gear::Renderer2D::BeginScene(introCamera);

			if (!inFirst && !isEndScene)
			{
				if (Gear::Input::IsKeyPressd(GR_KEY_ENTER) || Gear::Input::IsKeyPressd(GR_KEY_ESCAPE) || Gear::Input::IsKeyPressd(GR_KEY_SPACE)
					|| Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT) || Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT) )
				{
					isEndScene = true;
				}
			}

			if (isEndScene && !inFirst)
			{
				alpha.a -= 0.03f;
				Gear::Renderer2D::DrawQuad(introTranslate, introTexture, alpha);
				if (alpha.a <= 0.0f)
				{
					Gear::SceneManager::Get()->AddScene(new Intro::IntroScene2);
					Gear::SceneManager::Get()->changeScene("IntroScene2");
				}
			}
			else if (inFirst)
			{
				alpha.a += 0.05f;
				if (alpha.a >= 1.0f)
				{
					alpha.a = 1.0f;
					inFirst = false;
				}
				Gear::Renderer2D::DrawQuad(introTranslate, introTexture, alpha);
			}
			else
			{
				Gear::Renderer2D::DrawQuad(introTranslate, introTexture);
				pastTime += ts;
				if (pastTime >= sceneChangeDelay)
				{
					isEndScene = true;
				}
			}

			Gear::Renderer2D::EndScene();
		}

		void ChangeScene()
		{
			////Load Sound

			////Create Event Channel
			//Gear::EventSystem::CreateChannel(EventChannel::World);
			//Gear::EventSystem::CreateChannel(EventChannel::Explosion);
			//Gear::EventSystem::CreateChannel(EventChannel::Worm);
			//Gear::EventSystem::CreateChannel(EventChannel::MouseMove);
			//Gear::EventSystem::CreateChannel(EventChannel::MouseClick);
			//Gear::EventSystem::CreateChannel(EventChannel::Timer);

			////Temporary initData
			//InGame::InitiateData initData;
			//initData.Mapinfo = InGame::GetMapInfo("City");
			//std::string names[] = { "Sunwoo", "Younghwan", "TaeHwan", "Meongcheriya", "Chanho..TT", "Junsoo" };

			//std::vector<InGame::ItemInfo::ItemDescprition> itemList;
			//std::vector<InGame::ItemInfo::ItemDescprition> itemList2;

			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Bazooka, "Bazooka", -1, 0));
			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Grenade, "Grenade", -1, 0));
			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Shotgun, "Shotgun", -1, 0));
			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Banana, "Banana", 3, 0));
			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Teleport, "Teleport", -1, 0));
			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SkipGo, "Skip Go", -1, 0));
			//itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Surrender, "Surrender", -1, 0));


			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::PneumaticDril, "Pneumatic Dril", -1, 0));
			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::LowGravity, "Low Gravity", -1, 0));
			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::FirePunch, "Fire Punch", -1, 0));
			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Dynamite, "Dynamite", -1, 0));
			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::AirStrike, "Air Strike", -1, 0));
			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::BlowTorch, "Blow Torch", -1, 0));
			////itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::NinjaRope, "Ninja Rope", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SupperBanana, "Super Banana", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::PetrolBomb, "Petrol Bomb", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::MadCow, "Mad Cow", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::HomingMissile, "Homing Missile", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::HomingPigeon, "Homing Pigeon", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SheepLauncher, "Sheep Launcher", -1, 0));
			//itemList2.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Mortar, "Mortar", -1, 0));

			//InGame::TeamInfo team1;
			//InGame::TeamInfo team2;
			//team1.TeamName = "IL";
			//team1.TeamColor = InGame::TeamColor::Blue;
			//team1.nWorm = 3;
			//team1.TeamIcon = Gear::TextureStorage::GetTexture2D("Japan");
			//team1.TeamItem = itemList;

			//team2.TeamName = "KYUNG";
			//team2.TeamColor = InGame::TeamColor::Red;
			//team2.nWorm = 3;
			//team2.TeamIcon = Gear::TextureStorage::GetTexture2D("USA");
			//team2.TeamItem = itemList;


			//initData.Teams.push_back(team1);
			//initData.Teams.push_back(team2);

			//for (int i = 0; i < initData.Teams.size(); ++i)
			//{
			//	for (int j = 0; j < initData.Teams[i].nWorm; ++j)
			//	{
			//		InGame::WormSpecific worm;
			//		int flatIndex = i * initData.Teams[i].nWorm + j;
			//		worm.Name = names[i * initData.Teams[i].nWorm + j];
			//		worm.AdditionalZRenderOffset = flatIndex * 0.02f;
			//		worm.StartPosition = glm::vec3(Gear::Util::GetRndFloatFromTo(initData.Mapinfo.TerrainMinX, initData.Mapinfo.TerrainMaxX), 5.0f, ZOrder::z_Worm);
			//		worm.Hp = initData.WormMaxHP;
			//		worm.Direction = (InGame::WormInfo::DirectionType)Gear::Util::GetRndInt(2);

			//		initData.Teams[i].TotalWormHp += worm.Hp;
			//		initData.Teams[i].worms.push_back(worm);
			//		initData.nTotalWorms++;
			//	}
			//	initData.Teams[i].CurrentTotalWormHp = initData.Teams[i].TotalWormHp;
			//}

			////Create InGame Scene
			//Gear::SceneManager::Get()->AddScene(new InGame::InGameScene("InGameScene", nullptr, initData));
		}

	};
}