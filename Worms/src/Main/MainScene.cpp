#include "wmpch.h"
#include "MainScene.h"

#include "Loading/LoadingScene.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace Main {





	void MainScene::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(introCamera);
		Gear::Renderer2D::DrawQuad(gradTransform, Grad);

		if (mouseOnSingle)
		{
			Gear::Renderer2D::DrawQuad(singleTransform, SingleReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(singleTransform, Single);
		}

		if (mouseOnNet)
		{
			Gear::Renderer2D::DrawQuad(netTransform, NetReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(netTransform, Net);
		}

		if (mouseOnMulti)
		{
			Gear::Renderer2D::DrawQuad(multiTransform, MultiReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(multiTransform, Multi);
		}

		if (mouseOnOptions)
		{
			Gear::Renderer2D::DrawQuad(optionsTransform, OptionsReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(optionsTransform, Options);
		}

		if (mouseOnQuit)
		{
			Gear::Renderer2D::DrawQuad(quitTransform, QuitReady);
			Font::PrintFont(glm::vec3(0.73f, -0.8f, 0.4f), glm::vec3(0.1f, 0.1f, 1.0f), "Quit", FontType::WhiteSmall, 0.055f, false);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(quitTransform, Quit);
			Font::PrintFont(glm::vec3(0.73f, -0.8f, 0.4f), glm::vec3(0.1f, 0.1f, 1.0f), "Quit", FontType::GraySmall, 0.055f, false);
		}

		Gear::Renderer2D::DrawQuad(logoTransform, Logo);

		auto mousePos = Gear::Input::GetMousePosition();
		if (mousePos != centerMousePos)
		{
			cursorTransform[3][0] += (mousePos.first - centerMousePos.first) * 0.0013f;
			cursorTransform[3][1] += (centerMousePos.second - mousePos.second) * 0.00221f;

			if (cursorTransform[3][0] >= 1.37f)
			{
				cursorTransform[3][0] = 1.37f;
			}
			if (cursorTransform[3][0] <= -1.28f)
			{
				cursorTransform[3][0] = -1.28f;
			}
			if (cursorTransform[3][1] >= 0.98f)
			{
				cursorTransform[3][1] = 0.98f;
			}
			if (cursorTransform[3][1] <= -1.0f)
			{
				cursorTransform[3][1] = -1.0f;
			}
			virtualCursorPos.first = cursorTransform[3][0] + 0.01f;
			virtualCursorPos.second = cursorTransform[3][1] + 0.01f;
		}

		mouseOnSingle = false;
		mouseOnNet = false;
		mouseOnMulti = false;
		mouseOnOptions = false;
		mouseOnQuit = false;

		if (Gear::Util::IsPointRectCollision(virtualCursorPos, singleRect))
		{
			mouseOnSingle = true;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, netRect))
		{
			mouseOnNet = true;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, multiRect))
		{
			mouseOnMulti = true;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, optionRect))
		{
			mouseOnOptions = true;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, quitRect))
		{
			mouseOnQuit = true;
		}

		for (int i = 0; i < MaxStar; ++i)
		{
			starInfos[i].Update(ts);
			Gear::Renderer2D::DrawFrameQuad(starInfos[i].Transform, Star, 0, starInfos[i].Index);
		}

		glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);
		Gear::Renderer2D::DrawFrameQuad(cursorTransform, Cursor, 0, 0);

		if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
		{
			if (mouseOnQuit)
			{
				Gear::Application::Get().Quit();
			}
			if (mouseOnMulti)
			{
				StartInGame();
			}
		}

		if (inFirst)
		{
			Gear::Renderer2D::DrawQuad(alphaTransform, alphaColor);
			alphaColor.a -= 0.01f;
			if (alphaColor.a <= 0.0f)
			{
				inFirst = false;
			}
		}

		Gear::Renderer2D::EndScene();
	}

	void MainScene::StartInGame()
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