#include "wmpch.h"
#include "MultiScene.h"

#include "Loading/LoadingScene.h"
#include "GLFW/include/GLFW/glfw3.h"

namespace Main {
	void MapListLayer::OnEvent(Gear::Event & e)
	{
		Gear::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Gear::MouseScrolledEvent>(GR_BIND_EVENT_FN(MapListLayer::OnMouseScrolled));
		dispatcher.Dispatch<Gear::MouseButtonReleasedEvent>(GR_BIND_EVENT_FN(MapListLayer::OnMouseClick));
	}

	bool MapListLayer::OnMouseClick(Gear::MouseButtonReleasedEvent& e)
	{
		
		return true;
	}

	bool MapListLayer::OnMouseScrolled(Gear::MouseScrolledEvent & e)
	{
		if (!*MouseOnList)
			return true;

		float yOffset = e.GetYOffset();
		if (yOffset > 0.0f)
		{
			(*ScrollerTransform)[3][1] += ScrollerMoveUnit;
			if ((*ScrollerTransform)[3][1] > ScrollerTop)
			{
				(*ScrollerTransform)[3][1] = ScrollerTop;
			}
		}
		else if (yOffset < 0.0f)
		{
			(*ScrollerTransform)[3][1] -= ScrollerMoveUnit;
			if ((*ScrollerTransform)[3][1] < ScrollerBottom)
			{
				(*ScrollerTransform)[3][1] = ScrollerBottom;
			}
		}
		(*ScrollerRect).Set(*ScrollerTransform);
		calcListShowIndex();
		return true;
	}

	void MapListLayer::KeyInputLogic()
	{
		if (curListShowIndex <= highlighterIndex && highlighterIndex < curListShowIndex + 12)
		{
			if (Gear::Input::IsKeyPressd(GR_KEY_UP))
			{
				keypressReady = false;
				if (highlighterIndex != 0)
				{
					if (GetShowIndex(highlighterIndex) == 0)
					{
						--curListShowIndex;
						(*ScrollerTransform)[3][1] += ScrollerMoveUnit;
					}
					--highlighterIndex;
				}
			}

			if (Gear::Input::IsKeyPressd(GR_KEY_DOWN))
			{
				keypressReady = false;
				if (highlighterIndex != listIndexMax - 1)
				{
					if (GetShowIndex(highlighterIndex) == 11)
					{
						++curListShowIndex;
						(*ScrollerTransform)[3][1] -= ScrollerMoveUnit;
					}
					++highlighterIndex;
				}
			}
		}
		else
		{
			if (Gear::Input::IsKeyPressd(GR_KEY_UP))
			{
				keypressReady = false;
				if (highlighterIndex == 0)
				{
					curListShowIndex = 0;
					(*ScrollerTransform)[3][1] = ScrollerTop;
				}
				else
				{
					if (curListShowIndex < highlighterIndex)
					{
						--highlighterIndex;
						curListShowIndex = highlighterIndex - 11;
						(*ScrollerTransform)[3][1] = ScrollerTop - ScrollerMoveUnit * curListShowIndex;
					}
					else
					{
						--highlighterIndex;
						curListShowIndex = highlighterIndex;
						(*ScrollerTransform)[3][1] = ScrollerTop - ScrollerMoveUnit * curListShowIndex;
					}
				}
			}

			if (Gear::Input::IsKeyPressd(GR_KEY_DOWN))
			{
				keypressReady = false;
				if (highlighterIndex == listIndexMax - 1)
				{
					curListShowIndex = maxListShowIndex;
					(*ScrollerTransform)[3][1] = ScrollerBottom;
				}
				else
				{
					if (curListShowIndex < highlighterIndex)
					{
						++highlighterIndex;
						curListShowIndex = highlighterIndex - 11;
						(*ScrollerTransform)[3][1] = ScrollerTop - ScrollerMoveUnit * curListShowIndex;
					}
					else
					{
						++highlighterIndex;
						curListShowIndex = highlighterIndex;
						(*ScrollerTransform)[3][1] = ScrollerTop - ScrollerMoveUnit * curListShowIndex;
					}
				}
			}
		}
	}

	void MapListLayer::MousePressLogic()
	{
		if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
		{
			MousePressReady = false;
			for (int i = 0; i < showMapNameMax; ++i)
			{
				if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, fieldRects[i]))
				{
					int listIndex = GetListIndex(i);
					if (highlighterIndex == listIndex)
					{
						MultiScene::OnMapSelectorActive = false;
					}
					else
					{
						highlighterIndex = listIndex;
					}
				}
			}
		}
		
	}

	void BarracksLayer::sortTeamInfoListPoint()
	{
		std::sort(teamInfolist.begin(), teamInfolist.end(),
			[](const BasicTeamInfo* t1, const BasicTeamInfo* t2)
			{
				if (t1->points > t2->points)
				{
					return true;
				}
				else if (t1->points < t2->points)
				{
					return false;
				}

				if (t1->playerType < t2->playerType)
				{
					return true;
				}
				else if (t1->playerType > t2->playerType)
				{
					return false;
				}
				else
				{
					int team1NameLength = t1->teamName.size();
					int team2NameLength = t2->teamName.size();
					int cmpLength = team1NameLength < team2NameLength ? team1NameLength : team2NameLength;

					for (int i = 0; i < cmpLength; ++i)
					{
						if (t1->teamName[i] < t2->teamName[i])
						{
							return true;
						}
						else if (t1->teamName[i] > t2->teamName[i])
						{
							return false;
						}
					}
					return team1NameLength == cmpLength;
				}
				return true;
			}
		);
	}

	void BarracksLayer::sortTeamInfoListName()
	{
		std::sort(teamInfolist.begin(), teamInfolist.end(),
			[](const BasicTeamInfo* t1, const BasicTeamInfo* t2)
			{
				if (t1->playerType < t2->playerType)
				{
					return true;
				}
				else if (t1->playerType > t2->playerType)
				{
					return false;
				}
				else
				{
					int team1NameLength = t1->teamName.size();
					int team2NameLength = t2->teamName.size();
					int cmpLength = team1NameLength < team2NameLength ? team1NameLength : team2NameLength;

					for (int i = 0; i < cmpLength; ++i)
					{
						if (t1->teamName[i] < t2->teamName[i])
						{
							return true;
						}
						else if (t1->teamName[i] > t2->teamName[i])
						{
							return false;
						}
					}
					return team1NameLength == cmpLength;
				}
				return true;
			}
		);
	}

	void BarracksLayer::OnEvent(Gear::Event & e)
	{
		if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, BorderRect))
		{
			Gear::EventDispatcher dispatcher(e);
			dispatcher.Dispatch<Gear::MouseScrolledEvent>(GR_BIND_EVENT_FN(BarracksLayer::OnMouseScrolled));
			dispatcher.Dispatch<Gear::MouseButtonPressedEvent>(GR_BIND_EVENT_FN(BarracksLayer::OnMouseClick));
		}
	}

	bool BarracksLayer::OnMouseScrolled(Gear::MouseScrolledEvent & e)
	{
		if (e.GetYOffset() < 0)
		{
			if (curListShowIndex < listShowIndexMax)
			{
				++curListShowIndex;
				scrollerTransform[3][1] -= scrollerYOffset;
			}
		}
		else
		{
			if (curListShowIndex > 0)
			{
				--curListShowIndex;
				scrollerTransform[3][1] += scrollerYOffset;
			}
		}

		return true;
	}

	bool BarracksLayer::OnMouseClick(Gear::MouseButtonPressedEvent & e)
	{
		int indexMax = 6;
		if (teamInfolist.size() < indexMax)
		{
			indexMax = teamInfolist.size();
		}

		for (int i = 0; i < indexMax; ++i)
		{
			if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, filedRects[i]))
			{
				MultiScene::selectedTeamList.push_back(teamInfolist[i + curListShowIndex]);
				teamInfolist.erase(teamInfolist.begin() + i + curListShowIndex);
				RecalculateScrollerPos();
				break;
			}
		}
		return true;
	}

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

	std::pair<float, float> MultiScene::virtualCursorPos = { 0.0f, 0.0f };
	bool MultiScene::OnMapSelectorActive = false;
	std::vector<BasicTeamInfo*> MultiScene::selectedTeamList = std::vector<BasicTeamInfo*>();

	void MultiScene::CursorUpdate()
	{
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
			virtualCursorPos.first = cursorTransform[3][0] - 0.05f;
			virtualCursorPos.second = cursorTransform[3][1] + 0.01f;
		}
		glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);
	}

	void MultiScene::DrawButtons()
	{
		Gear::Renderer2D::DrawQuad(mapIconTransform, MapIcons[mapSelectedIndex]);
		Gear::Renderer2D::DrawQuad(selectedTeamTransform, SelectedTeam);

		if (MouseOn[Main_Multi::MapSelector] && !OnMapSelectorActive)
		{
			Gear::Renderer2D::DrawQuad(mapSelectorTransform, MapSelectorReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(mapSelectorTransform, MapSelector);
		}

		if (OnMapSelectorActive)
		{
			Gear::Renderer2D::DrawQuad(mapSelectorListTransform, MapSelectorList);
			Gear::Renderer2D::DrawQuad(mapSelectorScrollerTransform, MapSelectorScroller);
		}

		if (MouseOn[Main_Multi::GameSchemeSelector])
		{
			Gear::Renderer2D::DrawQuad(schemeSelectorTransform, SchemeSelectorReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(schemeSelectorTransform, SchemeSelector);
		}

		if (MouseOn[Main_Multi::TeamName])
		{
			Gear::Renderer2D::DrawQuad(teamListTransform, TeamListName);
		}
		else if (MouseOn[Main_Multi::Points])
		{
			Gear::Renderer2D::DrawQuad(teamListTransform, TeamListPoints);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(teamListTransform, TeamList);
		}

		if (MouseOn[Main_Multi::CreateNewTeam])
		{
			Gear::Renderer2D::DrawQuad(createTeamTransform, CreateTeamReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(createTeamTransform, CreateTeam);
		}

		if (MouseOn[Main_Multi::WeaponsOption])
		{
			Gear::Renderer2D::DrawQuad(weaponOptionTransform, WeaponsOptionReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(weaponOptionTransform, WeaponsOption);
		}

		if (MouseOn[Main_Multi::GameOption])
		{
			Gear::Renderer2D::DrawQuad(gameOptionTransform, GameOptionReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(gameOptionTransform, GameOption);
		}

		if (MouseOn[Main_Multi::Delete])
		{
			Gear::Renderer2D::DrawQuad(deleteTransform, DeleteReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(deleteTransform, Delete);
		}

		if (MouseOn[Main_Multi::Save])
		{
			Gear::Renderer2D::DrawQuad(saveTransform, DeleteReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(saveTransform, Delete);
		}

		if (MouseOn[Main_Multi::StartGame])
		{
			Gear::Renderer2D::DrawQuad(startTransform, StartReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(startTransform, Start);
		}

		if (MouseOn[Main_Multi::Exit])
		{
			Gear::Renderer2D::DrawQuad(exitTransform, ExitReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(exitTransform, Exit);
		}

		glm::mat4 tempOptionTransform = optionTransform;
		for (int i = 0; i < BasicOption::OptionMax; ++i)
		{
			if (i)
			{
				tempOptionTransform[3][0] += OptionsOffset;
			}
			if (MouseOn[i])
			{
				Gear::Renderer2D::DrawQuad(tempOptionTransform, OptionsReady[i][OptionsIndex[i]]);
			}
			else
			{
				Gear::Renderer2D::DrawQuad(tempOptionTransform, Options[i][OptionsIndex[i]]);
			}
		}
	}

	void MultiScene::DrawFont()
	{
		//Terrain
		auto mapName = Map::GetName(mapSelectedIndex);
		Font::PrintFont(glm::vec3(-1.12f + mapName.length() * 0.5f * 0.04f, mapSelectorTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), mapName, FontType::GraySmall, 0.04f, false);
		Font::PrintFont(glm::vec3(TerrainBorderTransform[3][0] + 0.03f, TerrainBorderTransform[3][1] + TerrainBorderTransform[1][1] / 2, 0.55f), glm::vec3(0.07f, 0.07f, 1.0f), "Terrain", FontType::GraySmall, 0.04f, false);

		//Scheme
		Font::PrintFont(glm::vec3(SchemeBorderTransform[3][0] + 0.03f, SchemeBorderTransform[3][1] + SchemeBorderTransform[1][1] / 2, 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Scheme options", FontType::GraySmall, 0.04f, false);
		Font::PrintFont(glm::vec3(schemeSelectorTransform[3][0] - 0.37f, schemeSelectorTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "[Default]", FontType::GraySmall, 0.04f, false);
	
		//Teams
		Font::PrintFont(glm::vec3(TeamBorderTransform[3][0] + 0.03f, TeamBorderTransform[3][1] + TeamBorderTransform[1][1] / 2, 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Teams", FontType::GraySmall, 0.04f, false);
		
		//Baracks
		Font::PrintFont(glm::vec3(BarrackBorderTransform[3][0] + 0.03f, BarrackBorderTransform[3][1] + BarrackBorderTransform[1][1] / 2, 0.55f), glm::vec3(0.07f, 0.07f, 1.0f), "Barracks", FontType::GraySmall, 0.04f, false);
		Font::PrintFont(glm::vec3(createTeamTransform[3][0] + 0.03f, createTeamTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Create a new Team", FontType::GraySmall, 0.04f, false);
		Font::PrintFont(glm::vec3(0.25f, 0.193f, 0.53f), glm::vec3(0.05f, 0.05f, 1.0f), "Team Name", FontType::GrayTinySmall, 0.027f, false);
		Font::PrintFont(glm::vec3(1.03f, 0.193f, 0.53f), glm::vec3(0.05f, 0.05f, 1.0f), "Points", FontType::GrayTinySmall, 0.027f, false);

		//GameStart
		Font::PrintFont(glm::vec3(startTransform[3][0] + 0.03f, startTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Start the game", FontType::GraySmall, 0.04f, false);
		Font::PrintFont(glm::vec3(exitTransform[3][0] + 0.03f, exitTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Exit", FontType::GraySmall, 0.04f, false);
	
		//Save delete
		Font::PrintFont(glm::vec3(saveTransform[3][0] + 0.03f, saveTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Save", FontType::GraySmall, 0.04f, false);
		Font::PrintFont(glm::vec3(deleteTransform[3][0] + 0.03f, deleteTransform[3][1], 0.53f), glm::vec3(0.07f, 0.07f, 1.0f), "Delete", FontType::GraySmall, 0.04f, false);
	}

	void MultiScene::ButtonUpdate()
	{
		MouseOn.reset();

		MouseOnMapList = false;
		if (OnMapSelectorActive)
		{
			if (Gear::Util::IsPointRectCollision(virtualCursorPos, MapSelectorListRect))
			{
				MouseOn[Main_Multi::MapSelectorList] = true;
				MouseOnMapList = true;
				return;
			}
		}

		

		if (Gear::Util::IsPointRectCollision(virtualCursorPos, StartRect))
		{
			MouseOn[Main_Multi::StartGame] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, ExitRect))
		{
			MouseOn[Main_Multi::Exit] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, CreateTeamRect))
		{
			MouseOn[Main_Multi::CreateNewTeam] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, WeaponsOptionRect))
		{
			MouseOn[Main_Multi::WeaponsOption] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, GameOptionRect))
		{
			MouseOn[Main_Multi::GameOption] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, MapSelectorRect))
		{
			MouseOn[Main_Multi::MapSelector] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, SchemeSelectorRect))
		{
			MouseOn[Main_Multi::GameSchemeSelector] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, SaveRect))
		{
			MouseOn[Main_Multi::Save] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, DeleteRect))
		{
			MouseOn[Main_Multi::Delete] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, SelectedTeamRect))
		{
			MouseOn[Main_Multi::Teams] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, TeamListNameRect))
		{
			MouseOn[Main_Multi::TeamName] = true;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, TeamListPointsRect))
		{
			MouseOn[Main_Multi::Points] = true;
			return;
		}
		for (int i = 0; i < BasicOption::OptionMax; ++i)
		{
			if (Gear::Util::IsPointRectCollision(virtualCursorPos, OptionsRect[i]))
			{
				MouseOn[i] = true;
			}
		}

	}


	void MultiScene::MouseButtonClick()
	{
		unsigned int ReadyButton;

		for (int i = 0; i < MouseOn.size(); ++i)
		{
			if (MouseOn[i])
			{
				ReadyButton = i;
				break;
			}
		}

		switch (ReadyButton)
		{
		case Main_Multi::TurnTime:
			ChangeTurnTime();
			break;
		case Main_Multi::RoundTime:
			ChangeRoundTime();
			break;
		case Main_Multi::WinRequires:
			ChangeWinRequires();
			break;
		case Main_Multi::WormSelect:
			ChangeWormSelect();
			break;
		case Main_Multi::WormEnergy:
			ChangeWormEenergy();
			break;
		case Main_Multi::Teleportin:
			ChangeTeleportin();
			break;
		case Main_Multi::Map:
			break;
		case Main_Multi::MapSelector:
			OnMapSelector();
			break;
		case Main_Multi::MapSelectorList:
			MapChange();
			break;
		case Main_Multi::Teams:
			OnTeams();
			break;
		case Main_Multi::WeaponsOption:
			OnWeaponsSet();
			break;
		case Main_Multi::GameOption:
			OnGameOptionSet();
			break;
		case Main_Multi::GameSchemeSelector:
			OnSchemeSelector();
			break;
		case Main_Multi::Delete:
			OnDelete();
			break;
		case Main_Multi::Save:
			OnSave();
			break;
		case Main_Multi::Barraks:
			OnBarracks();
			break;
		case Main_Multi::TeamName:
			OnTeamName();
			break;
		case Main_Multi::Points:
			OnPoints();
			break;
		case Main_Multi::CreateNewTeam:
			OnCreateNewTeam();
			break;
		case Main_Multi::StartGame:
			StartGame();
			break;
		case Main_Multi::Exit:
			OnExit();
			break;
		}
	}

	void MultiScene::ChangeTurnTime()
	{
		++OptionsIndex[BasicOption::Options::TurnTime];
		if (OptionsIndex[BasicOption::Options::TurnTime] >= BasicOption::TurnTime::TTMax)
		{
			OptionsIndex[BasicOption::Options::TurnTime] = 0;
		}
	}

	void MultiScene::ChangeRoundTime()
	{
		++OptionsIndex[BasicOption::Options::RoundTime];
		if (OptionsIndex[BasicOption::Options::RoundTime] >= BasicOption::RoundTime::RTMax)
		{
			OptionsIndex[BasicOption::Options::RoundTime] = 0;
		}
	}

	void MultiScene::ChangeWinRequires()
	{
		++OptionsIndex[BasicOption::Options::WinRequires];
		if (OptionsIndex[BasicOption::Options::WinRequires] >= BasicOption::WinRequires::WRMax)
		{
			OptionsIndex[BasicOption::Options::WinRequires] = 0;
		}
	}

	void MultiScene::ChangeWormSelect()
	{
		++OptionsIndex[BasicOption::Options::WormSelect];
		if (OptionsIndex[BasicOption::Options::WormSelect] >= BasicOption::WormSelect::WSRnd + 1)
		{
			OptionsIndex[BasicOption::Options::WormSelect] = 0;
		}
	}

	void MultiScene::ChangeWormEenergy()
	{
		++OptionsIndex[BasicOption::Options::WormEnergy];
		if (OptionsIndex[BasicOption::Options::WormEnergy] >= BasicOption::WormEnergy::WEMax)
		{
			OptionsIndex[BasicOption::Options::WormEnergy] = 0;
		}
	}

	void MultiScene::ChangeTeleportin()
	{
		++OptionsIndex[BasicOption::Options::Teleportin];
		if (OptionsIndex[BasicOption::Options::Teleportin] >= BasicOption::Teleportin::Off + 1)
		{
			OptionsIndex[BasicOption::Options::Teleportin] = 0;
		}
	}

	void MultiScene::OnMapSelector()
	{
		OnMapSelectorActive = !OnMapSelectorActive;
	}

	void MultiScene::MapChange()
	{
	}

	void MultiScene::OnTeams()
	{
	}

	void MultiScene::OnWeaponsSet()
	{
	}

	void MultiScene::OnGameOptionSet()
	{
	}

	void MultiScene::OnSchemeSelector()
	{
	}

	void MultiScene::OnDelete()
	{
	}

	void MultiScene::OnSave()
	{
	}

	void MultiScene::OnBarracks()
	{
	}

	void MultiScene::OnTeamName()
	{
	}

	void MultiScene::OnPoints()
	{
	}

	void MultiScene::OnCreateNewTeam()
	{
	}

	void MultiScene::OnExit()
	{
		Gear::SceneManager::Get()->changeScene("MainScene", 0);
	}

	void MultiScene::StartGame()
	{
	}



	

}