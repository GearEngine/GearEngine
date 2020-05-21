#include "wmpch.h"
#include "MainScene.h"

#include "Loading/LoadingScene.h"
#include "MultiScene.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace Main {

	void MainScene::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);
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
			virtualCursorPos.first = cursorTransform[3][0] - 0.01f;
			virtualCursorPos.second = cursorTransform[3][1] + 0.01f;
		}

		mouseOnSingle = false;
		mouseOnNet = false;
		mouseOnMulti = false;
		mouseOnOptions = false;
		mouseOnQuit = false;
		isOnButton = false;

		if (Gear::Util::IsPointRectCollision(virtualCursorPos, singleRect))
		{
			mouseOnSingle = true;
			isOnButton = true;
			if (prevOnCursorButton != 0)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 0;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, netRect))
		{
			mouseOnNet = true;
			isOnButton = true;
			if (prevOnCursorButton != 1)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 1;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, multiRect))
		{
			mouseOnMulti = true;
			isOnButton = true;
			if (prevOnCursorButton != 2)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 2;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, optionRect))
		{
			mouseOnOptions = true;
			isOnButton = true;
			if (prevOnCursorButton != 3)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 3;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, quitRect))
		{
			mouseOnQuit = true;
			isOnButton = true;
			if (prevOnCursorButton != 4)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 4;
		}

		if (!isOnButton)
		{
			prevOnCursorButton = -1;
		}


		for (int i = 0; i < SceneBackground::MaxStar; ++i)
		{
			starInfos[i].Update(ts);
			Gear::Renderer2D::DrawFrameQuad(starInfos[i].Transform, Star, 0, starInfos[i].Index);
		}

		glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);
		Gear::Renderer2D::DrawFrameQuad(cursorTransform, Cursor, 0, 0);

		if (!mouseClickReady)
		{
			pastTime += ts;
			if (pastTime > mouseClickDelay)
			{
				pastTime = 0.0f;
				mouseClickReady = true;
			}
		}

		if (mouseClickReady)
		{
			if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
			{
				mouseClickReady = false;
				if (mouseOnQuit)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					Gear::Application::Get().Quit();
				}
				if (mouseOnMulti)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					if (Gear::SceneManager::Get()->isSceneExist("MultiScene"))
					{
						Gear::SceneManager::Get()->changeScene("MultiScene", 0);
					}
					else
					{
						Gear::SceneManager::Get()->AddScene(new MultiScene);
						Gear::SceneManager::Get()->changeScene("MultiScene");
					}
				}
				if (mouseOnNet)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					StartNetwork();
				}
				if (mouseOnOptions)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
				}
				if (mouseOnSingle)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
				}

				if (!isOnButton)
				{
					PLAY_SOUND_NAME("cantclickhere", WormsSound::effect);
				}
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

	void MainScene::StartNetwork()
	{
		Gear::NetWorkManager::Get()->ConnectServer("127.0.0.1:9190");
		auto start = Gear::NetWorkManager::Get()->Receive<WormsPacket::StartPacket>();

		InGame::InitiateData initData;
		initData.GameMode = GameMode::NetWork;
		GameMode::Bit::ModeBit = GameMode::NetWork;
		Gear::EntitySystem::SetNetwork(true);
		GameMode::Bit::NetID = start.netID;
		std::cout << "Receive Start Packet - NetID" << start.netID << std::endl;

		srand(100);

		//clear
		initData.Teams.clear();
		initData.Mapinfo.Map.reset();
		initData.Mapinfo.Mask.reset();

		//MapSetting
		initData.Mapinfo = InGame::GetMapInfo(Map::GetName(Map::City));
		initData.Mapinfo.MapLoad();

		//ItemSetting
		std::vector<InGame::ItemInfo::ItemDescprition> itemList;
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Bazooka, "Bazooka", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Grenade, "Grenade", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Shotgun, "Shotgun", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Banana, "Banana", 3, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Teleport, "Teleport", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::SkipGo, "Skip Go", -1, 0));
		itemList.push_back(InGame::ItemInfo::ItemDescprition(InGame::ItemInfo::Surrender, "Surrender", -1, 0));

		//BagicGameOption
		initData.LimitTurnTime = BasicOption::GetTurnTime(BasicOption::TurnTime::TT45);

		//Team and Worms Setting
		int flatCount = 0;
		auto rng = std::default_random_engine{};
		int wormEnergy = BasicOption::GetWormEnergy(BasicOption::WormEnergy::WE150);

		float xArr[6] = { 20.0f, -15.0f, 13.0f, 1.0f, 8.0f, -10.0f };

		for (int i = 0; i < 2; ++i)
		{
			InGame::TeamInfo team;
			team.TeamName = std::to_string(i + 1) + "-Up";
			team.TeamColor = InGame::TeamColor::Color(i + 19);
			team.nWorm = 3;
			team.TeamIcon = Gear::TextureStorage::GetTexture2D("Japan");
			team.TeamItem = itemList;

			std::vector<std::string> nameList = {"Worm1", "Worm2", "Worm3"};
			std::shuffle(nameList.begin(), nameList.end(), rng);
			for (int j = 0; j < team.nWorm; ++j)
			{
				InGame::WormSpecific worm;
				worm.Name = nameList[j];
				worm.AdditionalZRenderOffset = flatCount * 0.02f;
				worm.StartPosition = glm::vec3(xArr[i * 3 + j], 5.0f, ZOrder::z_Worm);
				worm.Direction = (InGame::WormInfo::DirectionType)Gear::Util::GetRndInt(2);
				worm.Hp = wormEnergy;
				if (i == GameMode::Bit::NetID)
				{
					worm.MyNetCharacter = true;
				}
				team.TotalWormHp += worm.Hp;
				team.worms.push_back(worm);
				++initData.nTotalWorms;
				++flatCount;
			}
			team.CurrentTotalWormHp = team.TotalWormHp;
			initData.Teams.push_back(team);
		}
		
		Gear::EntitySystem::Init();
		Gear::SoundSystem::Get()->StopChannel(WormsSound::bgm);
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