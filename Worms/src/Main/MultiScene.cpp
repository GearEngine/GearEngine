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
				if (MultiScene::selectedTeamList.size() < 6)
				{
					PLAY_SOUND_NAME("positive", WormsSound::effect);
					MultiScene::selectedTeamList.push_back(teamInfolist[i + curListShowIndex]);
					teamInfolist.erase(teamInfolist.begin() + i + curListShowIndex);
					RecalculateScrollerPos();
				}
				break;
			}
		}
		return true;
	}

	void SelectedTeamLayer::DrawFont()
	{
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			std::string name = MultiScene::selectedTeamList[i]->Name;
			int length = name.length();
			Font::PrintFont(glm::vec3(filedTransform[3][0] - 0.2f + length * 0.5f * 0.024f, filedTransform[3][1] - i * filedYOffset, 0.4f), glm::vec3(0.05f, 0.05f, 1.0f), name, FontType::WhiteTinySmall, 0.025f, false);
		}
	}

	void SelectedTeamLayer::DrawIcon()
	{
		glm::mat4 tempTransform = playerTypeIconTransform;
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (i)
			{
				tempTransform[3][1] -= filedYOffset;
			}
			Gear::Renderer2D::DrawQuad(tempTransform, playerTypeIcons[MultiScene::selectedTeamList[i]->playerType]);
		}
	}

	void SelectedTeamLayer::ButtonUpdate()
	{
		mouseOnHandicap.reset();
		mouseOnAlly.reset();
		mouseOnWormCount.reset();

		//Handicap
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, handicapRect[i]))
			{
				mouseOnHandicap[i] = true;
				break;
			}
		}

		//Ally
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, allyRect[i]))
			{
				mouseOnAlly[i] = true;
				break;
			}
		}

		//WormCount
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, wormCountRect[i]))
			{
				mouseOnWormCount[i] = true;
				break;
			}
		}
	}

	void SelectedTeamLayer::DrawButton()
	{
		//Handicap
		glm::mat4 tempHCTransform = handicapTransform;
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (i)
			{
				tempHCTransform[3][1] -= filedYOffset;
			}
			if (mouseOnHandicap[i])
			{
				Gear::Renderer2D::DrawQuad(tempHCTransform, handicapReadyTexture[handicapType[i]]);
			}
			else
			{
				Gear::Renderer2D::DrawQuad(tempHCTransform, handicapTexture[handicapType[i]]);
			}
		}

		//Ally
		glm::mat4 tempAYTransform = allyTransform;
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (i)
			{
				tempAYTransform[3][1] -= filedYOffset;
			}
			if (mouseOnAlly[i])
			{
				Gear::Renderer2D::DrawQuad(tempAYTransform, allyReadyTexture[allyType[i]]);
			}
			else
			{
				Gear::Renderer2D::DrawQuad(tempAYTransform, allyTexture[allyType[i]]);
			}
		}

		//WormCount
		glm::mat4 tempWCTransform = wormCountTransform;
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (i)
			{
				tempWCTransform[3][1] -= filedYOffset;
			}
			if (mouseOnWormCount[i])
			{
				Gear::Renderer2D::DrawQuad(tempWCTransform, wormCountReadyTexture[wormCount[i]]);
			}
			else
			{
				Gear::Renderer2D::DrawQuad(tempWCTransform, wormCountTexture[wormCount[i]]);
			}
		}
	}

	int SelectedTeamLayer::GetAllyCount()
	{
		std::set<unsigned int> test;
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			test.insert(allyType[i]);
		}
		return test.size();
	}

	void SelectedTeamLayer::OnEvent(Gear::Event & e)
	{
		Gear::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Gear::MouseButtonPressedEvent>(GR_BIND_EVENT_FN(SelectedTeamLayer::OnMouseClick));
	}

	bool SelectedTeamLayer::OnMouseClick(Gear::MouseButtonPressedEvent & e)
	{
		//handicapHandle
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (mouseOnHandicap[i])
			{
				if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					++handicapType[i];
					if (handicapType[i] >= TeamBasicOption::HMax)
					{
						handicapType[i] = 0;
					}
					return false;
				}
				if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT))
				{
					PLAY_SOUND_NAME("decreaseiconnumber", WormsSound::effect);
					--handicapType[i];
					if (handicapType[i] < 0)
					{
						handicapType[i] = TeamBasicOption::HMax - 1;
					}
					return false;
				}
			}
		}

		//AllyHandle
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (mouseOnAlly[i])
			{
				if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					++allyType[i];
					if (allyType[i] >= TeamBasicOption::AMax)
					{
						allyType[i] = 0;
					}
					return false;
				}
				if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT))
				{
					PLAY_SOUND_NAME("decreaseiconnumber", WormsSound::effect);
					--allyType[i];
					if (allyType[i] < 0)
					{
						allyType[i] = TeamBasicOption::AMax - 1;
					}
					return false;
				}
			}
		}

		//wormCountHandle
		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (mouseOnWormCount[i])
			{
				if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					if (wormCount[i] < TeamBasicOption::Max)
					{
						++wormCount[i];
					}
					return false;
				}
				if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT))
				{
					PLAY_SOUND_NAME("decreaseiconnumber", WormsSound::effect);
					if (wormCount[i] > 0)
					{
						--wormCount[i];
					}
					return false;
				}
			}
		}

		for (int i = 0; i < MultiScene::selectedTeamList.size(); ++i)
		{
			if (Gear::Util::IsPointRectCollision(MultiScene::virtualCursorPos, filedRects[i]))
			{
				PLAY_SOUND_NAME("changetoeditmode", WormsSound::effect);
				barracksLayer->pushTeamData(MultiScene::selectedTeamList[i]);
				barracksLayer->sortTeamInfoListName();
				MultiScene::selectedTeamList.erase(MultiScene::selectedTeamList.begin() + i);
				for (int j = i; j < MultiScene::selectedTeamList.size(); ++j)
				{
					wormCount[j] = wormCount[j + 1];
				}
				resetBasicInfo();
				return false;
			}
		}
		return false;
	}

	void SelectedTeamLayer::resetBasicInfo()
	{
		handicapType[MultiScene::selectedTeamList.size()] = TeamBasicOption::Handicap::None;
		allyType[MultiScene::selectedTeamList.size()] = MultiScene::selectedTeamList.size();
		wormCount[MultiScene::selectedTeamList.size()] = TeamBasicOption::WormCount::_3;
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
			if (pastMouseOn != 0)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 0;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, ExitRect))
		{
			MouseOn[Main_Multi::Exit] = true;
			if (pastMouseOn != 1)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 1;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, CreateTeamRect))
		{
			MouseOn[Main_Multi::CreateNewTeam] = true;
			if (pastMouseOn != 2)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 2;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, WeaponsOptionRect))
		{
			MouseOn[Main_Multi::WeaponsOption] = true;
			if (pastMouseOn != 3)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 3;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, GameOptionRect))
		{
			MouseOn[Main_Multi::GameOption] = true;
			if (pastMouseOn != 4)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 4;
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
			if (pastMouseOn != 5)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 5;
			return;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, DeleteRect))
		{
			MouseOn[Main_Multi::Delete] = true;
			if (pastMouseOn != 6)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			pastMouseOn = 6;
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
				if (pastMouseOn != 7 + i)
				{
					PLAY_SOUND_NAME("keyerase", WormsSound::effect);
				}
				pastMouseOn = 7 + i;
				MouseOn[i] = true;
				return;
			}
		}
		pastMouseOn = -1;
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
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			ChangeTurnTime();
			break;
		case Main_Multi::RoundTime:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			ChangeRoundTime();
			break;
		case Main_Multi::WinRequires:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			ChangeWinRequires();
			break;
		case Main_Multi::WormSelect:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			ChangeWormSelect();
			break;
		case Main_Multi::WormEnergy:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			ChangeWormEenergy();
			break;
		case Main_Multi::Teleportin:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
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
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			OnWeaponsSet();
			break;
		case Main_Multi::GameOption:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			OnGameOptionSet();
			break;
		case Main_Multi::GameSchemeSelector:
			OnSchemeSelector();
			break;
		case Main_Multi::Delete:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			OnDelete();
			break;
		case Main_Multi::Save:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
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
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			OnCreateNewTeam();
			break;
		case Main_Multi::StartGame:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			StartGame();
			break;
		case Main_Multi::Exit:
			PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
			OnExit();
			break;
		}
	}

	bool MultiScene::allyCheck()
	{
		if (selectedTeamLayer->GetAllyCount() >= 2)
		{
			return true;
		}
		return false;
	}

	bool MultiScene::SettingInitData()
	{
		if (!allyCheck())
		{
			return false;
		}

		initData.GameMode = GameMode::NetWork;
		GameMode::Bit::ModeBit = GameMode::NetWork;

		//clear
		initData.Teams.clear();
		initData.Mapinfo.Map.reset();
		initData.Mapinfo.Mask.reset();

		//MapSetting
		initData.Mapinfo = InGame::GetMapInfo(Map::GetName(mapSelectedIndex));
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
		initData.LimitTurnTime = BasicOption::GetTurnTime(OptionsIndex[BasicOption::Options::TurnTime]);

		//Team and Worms Setting
		int flatCount = 0;
		auto rng = std::default_random_engine{};
		int wormEnergy = BasicOption::GetWormEnergy(OptionsIndex[BasicOption::Options::WormEnergy]);
		for (int i = 0; i < selectedTeamList.size(); ++i)
		{
			InGame::TeamInfo team;
			team.TeamName = selectedTeamList[i]->Name;
			team.TeamColor = InGame::TeamColor::ConvertAllyToColor(selectedTeamLayer->allyType[i]);
			team.nWorm = selectedTeamLayer->wormCount[i] + 1;
			team.TeamIcon = Gear::TextureStorage::GetTexture2D(selectedTeamList[i]->teamIcon);
			team.TeamItem = itemList;

			auto coppyWormNameList = selectedTeamList[i]->wormName;
			std::shuffle(coppyWormNameList.begin(), coppyWormNameList.end(), rng);
			for (int j = 0; j < team.nWorm; ++j)
			{
				InGame::WormSpecific worm;
				worm.Name = selectedTeamList[i]->wormName[j];
				worm.AdditionalZRenderOffset = flatCount * 0.02f;
				worm.StartPosition = glm::vec3(Gear::Util::GetRndFloatFromTo(initData.Mapinfo.TerrainMinX, initData.Mapinfo.TerrainMaxX), 5.0f, ZOrder::z_Worm);
				worm.Direction = (InGame::WormInfo::DirectionType)Gear::Util::GetRndInt(2);
				worm.Hp = wormEnergy;
				if (selectedTeamLayer->handicapType[i] == TeamBasicOption::Handicap::Plus)
				{
					worm.Hp += 25;
				}
				if (selectedTeamLayer->handicapType[i] == TeamBasicOption::Handicap::Minus)
				{
					worm.Hp -= 25;
				}
				team.TotalWormHp += worm.Hp;
				team.worms.push_back(worm);
				++initData.nTotalWorms;
				++flatCount;
			}
			team.CurrentTotalWormHp = team.TotalWormHp;
			initData.Teams.push_back(team);
		}
		return true;
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
		if (!SettingInitData())
		{
			return;
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