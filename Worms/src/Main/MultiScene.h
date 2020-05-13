#pragma once

namespace Main {

	struct MainTeamInfo
	{
		std::string name;
		std::string teamIcon;

	};


	class MapListLayer : public Gear::Layer
	{
		std::vector<std::string> MapStrList;

		const float ScrollerTop = 0.3f;
		const float ScrollerBottom = -0.135f;
		const float listFieldTop = 0.382f;
		float fieldCenterX = -0.6748f;
		float fieldFront = -1.124f;
		float ScrollerMoveUnit;

		int curListShowIndex = 0;
		int maxListShowIndex;
		int listIndexMax;
		int showMapNameMax = 12;

		int highlighterIndex = 0;

		glm::mat4* ScrollerTransform;
		Gear::Util::FRect* ScrollerRect;
		bool* MouseOnList;

		std::vector<glm::mat4> fieldTransforms;
		std::vector<Gear::Util::FRect> fieldRects;

		float MapSelectorDivideUnit;

		//KeyDown
		float keyPastTime = 0.0f;
		float keyPressDeleay = 0.05f;
		bool keypressReady = true;

		//MousePress
		float mousePressPastTime = 0.0f;
		float mousePressDelay = 0.1f;
		bool MousePressReady = false;

		int* SelectedIndex;

	public:
		MapListLayer(glm::mat4* mapSelectorScrollerTransform, Gear::Util::FRect* scrollerRect, bool* mouseOnList, float mapselctorListHeight, float mapselectorListWidth, int* selectedIndex)
			: Gear::Layer("ListLayer")
		{
			MapStrList.resize(Map::Max);
			ScrollerMoveUnit = (ScrollerTop - ScrollerBottom) / (Map::Max - showMapNameMax);
			ScrollerTransform = mapSelectorScrollerTransform;
			ScrollerRect = scrollerRect;
			MouseOnList = mouseOnList;
			SelectedIndex = selectedIndex;
			*SelectedIndex = highlighterIndex;

			listIndexMax = Map::Max;
			maxListShowIndex = listIndexMax - showMapNameMax;
			MapSelectorDivideUnit = (mapselctorListHeight - 0.022f) / showMapNameMax;

			for (int i = 0; i < Map::Max; ++i)
			{
				MapStrList[i] = Map::GetName(i);
			}

			fieldTransforms.resize(showMapNameMax);
			fieldRects.resize(showMapNameMax);
			for (int i = 0; i < showMapNameMax; ++i)
			{
				fieldTransforms[i] = glm::translate(glm::mat4(1.0f), glm::vec3(fieldCenterX, listFieldTop - MapSelectorDivideUnit * i, 0.57f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapselectorListWidth - 0.07f, MapSelectorDivideUnit, 1.0f));
				fieldRects[i].Set(fieldTransforms[i]);
			}
		}
		
		int GetShowIndex(int curIndex)
		{
			return curIndex - curListShowIndex;
		}

		int GetListIndex(int showIndex)
		{
			return showIndex + curListShowIndex;
		}

		void calcListShowIndex()
		{
			float scrollerY = (*ScrollerTransform)[3][1];
			float dist = ScrollerTop - scrollerY;
			curListShowIndex = dist / ScrollerMoveUnit;
			if (curListShowIndex > maxListShowIndex)
			{
				curListShowIndex = maxListShowIndex - 13;
			}
		}

		virtual void OnUpdate(Gear::Timestep ts) override
		{
			if (!keypressReady)
			{
				keyPastTime += ts;
				if (keyPastTime > keyPressDeleay)
				{
					keyPastTime = 0.0f;
					keypressReady = true;
				}
			}

			if (!MousePressReady)
			{
				mousePressPastTime += ts;
				if (mousePressPastTime > mousePressDelay)
				{
					mousePressPastTime = 0.0f;
					MousePressReady = true;
				}
			}

			if (keypressReady)
			{
				KeyInputLogic();
			}

			if (MousePressReady)
			{
				MousePressLogic();
			}

			*SelectedIndex = highlighterIndex;

			if (curListShowIndex <= highlighterIndex && highlighterIndex < curListShowIndex + 12)
			{
				Gear::Renderer2D::DrawQuad(fieldTransforms[GetShowIndex(highlighterIndex)], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			for (int i = 0; i < showMapNameMax; ++i)
			{
				auto mapName = MapStrList[i + curListShowIndex];
				Font::PrintFont(glm::vec3(fieldFront + mapName.length() * 0.5f * 0.024f, fieldTransforms[i][3][1], 0.58f), glm::vec3(0.05f, 0.05f, 1.0f), mapName, FontType::WhiteTinySmall, 0.025f, false);
			}
		}

		virtual void OnEvent(Gear::Event& e) override;

		bool OnMouseScrolled(Gear::MouseScrolledEvent & e);
		bool OnMouseClick(Gear::MouseButtonReleasedEvent& e);

		void KeyInputLogic();
		void MousePressLogic();
	};

	class TeamSelectLayer : public Gear::Layer 
	{

	public:
		TeamSelectLayer()
			: Gear::Layer("TeamSelectLayer")
		{

		}

	};

	class MultiScene : public Gear::Scene
	{
		InGame::InitiateData initData;

		float reductionRatio = 180.0f;
		float reductionRatio2 = 238.0f;
		float reductionRatio3 = 240.0f;

		//Cursor
		Gear::Ref<Gear::FrameTexture2D> Cursor;
		glm::mat4 cursorTransform;
	public:
		static std::pair<float, float> virtualCursorPos;
		static bool OnMapSelectorActive;

	private:
		std::pair<float, float> centerMousePos;
		//

		Gear::Ref<Gear::Texture2D> DivideBorder;
		glm::mat4 TerrainBorderTransform;
		glm::mat4 TeamBorderTransform;

		Gear::Ref<Gear::Texture2D> DivideBorder2;
		glm::mat4 SchemeBorderTransform;
		glm::mat4 BarrackBorderTransform;

		Gear::Ref<Gear::Texture2D> Grad;
		glm::mat4 gradTransform;

		Gear::Ref<Gear::FrameTexture2D> Star;
		std::vector<SceneBackground::StarInfo> starInfos;

		//MapSelector
		Gear::Ref<Gear::Texture2D> MapSelector;
		Gear::Ref<Gear::Texture2D> MapSelectorReady;
		Gear::Util::FRect MapSelectorRect;
		glm::mat4 mapSelectorTransform;
		bool MouseOnMapList;

		Gear::Util::FRect MapSelectorListRect;
		glm::mat4 mapSelectorListTransform;
		Gear::Ref<Gear::Texture2D> MapSelectorList;

		Gear::Ref<Gear::Texture2D> MapSelectorScroller;
		Gear::Ref<Gear::Texture2D> MapSelectorScrollerReady;
		Gear::Util::FRect MapSelectorScrollerRect;

		glm::mat4 mapSelectorScrollerTransform;

		Gear::Layer* MapSelectorLayer;
		int mapSelectedIndex;
		//

		Gear::Ref<Gear::Texture2D> SchemeSelector;
		Gear::Ref<Gear::Texture2D> SchemeSelectorReady;
		Gear::Util::FRect SchemeSelectorRect;
		glm::mat4 schemeSelectorTransform;

		std::vector<Gear::Ref<Gear::Texture2D>> MapIcons;
		glm::mat4 mapIconTransform;

		Gear::Ref<Gear::Texture2D> SelectedTeam;
		Gear::Util::FRect SelectedTeamRect;
		glm::mat4 selectedTeamTransform;

		Gear::Ref<Gear::Texture2D> TeamList;
		Gear::Ref<Gear::Texture2D> TeamListName;
		Gear::Ref<Gear::Texture2D> TeamListPoints;
		Gear::Util::FRect TeamListRect;
		Gear::Util::FRect TeamListNameRect;
		Gear::Util::FRect TeamListPointsRect;
		glm::mat4 teamListTransform;

		Gear::Ref<Gear::Texture2D> CreateTeam;
		Gear::Ref<Gear::Texture2D> CreateTeamReady;
		Gear::Util::FRect CreateTeamRect;
		glm::mat4 createTeamTransform;

		Gear::Ref<Gear::Texture2D> WeaponsOption;
		Gear::Ref<Gear::Texture2D> WeaponsOptionReady;
		Gear::Util::FRect WeaponsOptionRect;
		glm::mat4 weaponOptionTransform;

		Gear::Ref<Gear::Texture2D> GameOption;
		Gear::Ref<Gear::Texture2D> GameOptionReady;
		Gear::Util::FRect GameOptionRect;
		glm::mat4 gameOptionTransform;

		Gear::Ref<Gear::Texture2D> Delete;
		Gear::Ref<Gear::Texture2D> DeleteReady;
		Gear::Util::FRect DeleteRect;
		glm::mat4 deleteTransform;

		Gear::Ref<Gear::Texture2D> Save;
		Gear::Ref<Gear::Texture2D> SaveReady;
		Gear::Util::FRect SaveRect;
		glm::mat4 saveTransform;

		Gear::Ref<Gear::Texture2D> Start;
		Gear::Ref<Gear::Texture2D> StartReady;
		Gear::Util::FRect StartRect;
		glm::mat4 startTransform;

		Gear::Ref<Gear::Texture2D> Exit;
		Gear::Ref<Gear::Texture2D> ExitReady;
		Gear::Util::FRect ExitRect;
		glm::mat4 exitTransform;

		std::vector<std::vector<Gear::Ref<Gear::Texture2D>>> Options;
		std::vector<std::vector<Gear::Ref<Gear::Texture2D>>> OptionsReady;
		std::vector<Gear::Util::FRect> OptionsRect;
		std::vector<int> OptionsIndex;
		std::vector<bool> isOptionReady;
		glm::mat4 optionTransform;
		float OptionsOffset = 0.21f;

		std::bitset<Main_Multi::Ready::Max> MouseOn;
		bool readyMouseClick = true;
		const float MouseClickDeleay = 0.2f;
		float mouseClickPastTime = 0.0f;

	public:
		MultiScene()
			: Scene("MultiScene"), centerMousePos(640.0f, 360.0f)
		{

			Grad = Gear::TextureStorage::GetTexture2D("minigrad");

			Cursor = Gear::TextureStorage::GetFrameTexture2D("Cursor");
			DivideBorder = Gear::TextureStorage::GetTexture2D("TerrainBorder");
			DivideBorder2 = Gear::TextureStorage::GetTexture2D("SchemeBorder");
			SelectedTeam = Gear::TextureStorage::GetTexture2D("SelectedTeam");
			TeamList = Gear::TextureStorage::GetTexture2D("TeamList");
			TeamListName = Gear::TextureStorage::GetTexture2D("TeamListName");
			TeamListPoints = Gear::TextureStorage::GetTexture2D("TeamListPoints");
			Star = Gear::TextureStorage::GetFrameTexture2D("FallenStar");
			MapSelector = Gear::TextureStorage::GetTexture2D("MapSelector");
			MapSelectorReady = Gear::TextureStorage::GetTexture2D("MapSelectorReady");
			MapSelectorList = Gear::TextureStorage::GetTexture2D("MapSelectorList");
			MapSelectorScroller = Gear::TextureStorage::GetTexture2D("MapListScroller");
			MapSelectorScrollerReady = Gear::TextureStorage::GetTexture2D("MapListScrollerr");
			SchemeSelector = Gear::TextureStorage::GetTexture2D("SchemeSelector");
			SchemeSelectorReady = Gear::TextureStorage::GetTexture2D("SchemeSelectorReady");
			CreateTeam = Gear::TextureStorage::GetTexture2D("CreateTeamButton");
			CreateTeamReady = Gear::TextureStorage::GetTexture2D("CreateTeamButtonReady");
			WeaponsOption = Gear::TextureStorage::GetTexture2D("WeaponsOption");
			WeaponsOptionReady = Gear::TextureStorage::GetTexture2D("WeaponsOptionReady");
			GameOption = Gear::TextureStorage::GetTexture2D("GameOption");
			GameOptionReady = Gear::TextureStorage::GetTexture2D("GameOptionReady");
			Delete = Gear::TextureStorage::GetTexture2D("Delete");
			DeleteReady = Gear::TextureStorage::GetTexture2D("DeleteReady");
			Exit = Gear::TextureStorage::GetTexture2D("MultiExit");
			ExitReady = Gear::TextureStorage::GetTexture2D("MultiExitReady");
			Start = Gear::TextureStorage::GetTexture2D("MultiStart");
			StartReady = Gear::TextureStorage::GetTexture2D("MultiStartReady");
			Cursor = Gear::TextureStorage::GetFrameTexture2D("Cursor");

			MapIcons.resize(Map::Max);

			for (int i = 0; i < Map::Max; ++i)
			{
				auto mapName = Map::GetName(i);
				MapIcons[i] = Gear::TextureStorage::GetTexture2D(mapName + "Icon");
			}

			Options.resize(BasicOption::OptionMax);
			OptionsReady.resize(BasicOption::OptionMax);
			OptionsIndex.resize(BasicOption::OptionMax);

			Init(0);

			for (int optionType = 0; optionType < BasicOption::OptionMax; ++optionType)
			{
				int variationMax = BasicOption::GetVaritaionCount(optionType);
				Options[optionType].resize(variationMax);
				OptionsReady[optionType].resize(variationMax);
				for (int variationType = 0; variationType < variationMax; ++variationType)
				{
					auto name = BasicOption::GetVariationName(optionType, variationType);
					Options[optionType][variationType] = Gear::TextureStorage::GetTexture2D(name);
					OptionsReady[optionType][variationType] = Gear::TextureStorage::GetTexture2D(name + 'r');
				}
			}

			float GradWidth = Grad->GetWidth();
			float divideBorderWidth = DivideBorder->GetWidth();
			float divideBorderHeight = DivideBorder->GetHeight();
			float divideBorder2Width = DivideBorder2->GetWidth();
			float divideBorder2Height = DivideBorder2->GetHeight();
			float mapSelectorWidth = MapSelector->GetWidth();
			float mapSelectorHeight = MapSelector->GetHeight();
			float mapIconWidth = MapIcons[0]->GetWidth();
			float mapIconHeight = MapIcons[0]->GetHeight();
			float selectedTeamWidth = SelectedTeam->GetWidth();
			float selectedTeamHeight = SelectedTeam->GetHeight();
			float teamListWidth = TeamList->GetWidth() - 20.0f;
			float teamListHeight = TeamList->GetHeight();
			float createTeamWidth = CreateTeam->GetWidth() - 20.0f;
			float createTeamHeight = CreateTeam->GetHeight();
			float gameOptionWidth = GameOption->GetWidth();
			float gameOptionHeight = GameOption->GetHeight();
			float deleteWidth = Delete->GetWidth();
			float deleteHeight = Delete->GetHeight();
			float exitWidth = Exit->GetWidth() - 10.0f;
			float exitHeight = Exit->GetHeight();
			float startWidth = Start->GetWidth() - 30.0f;
			float startHeight = Start->GetHeight();
			float optionWidth = Options[0][0]->GetWidth();
			float optionHeight = Options[0][0]->GetHeight();
			float schemeSelectorWidth = SchemeSelector->GetWidth();
			float schemeSelectorHeight = SchemeSelector->GetHeight();
			float mapSelectorListWidth = MapSelectorList->GetWidth();
			float mapSelectorListHeight = MapSelectorList->GetHeight();
			float mapSelectorScrollerWidth = MapSelectorScroller->GetWidth();
			float mapSelectorScrollerHeight = MapSelectorScroller->GetHeight();

			gradTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(GradWidth / reductionRatio, GradWidth / reductionRatio, 1.0f));
			TerrainBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.65f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorderWidth / reductionRatio2, divideBorderHeight / reductionRatio2, 1.0f));
			TeamBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, 0.65f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorderWidth / reductionRatio2, divideBorderHeight / reductionRatio2, 1.0f));
			SchemeBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, -0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorder2Width / reductionRatio2, divideBorder2Height / reductionRatio2, 1.0f));
			BarrackBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorder2Width / reductionRatio2, divideBorder2Height / reductionRatio2, 1.0f));
			mapSelectorTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.46f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapSelectorWidth / reductionRatio2, mapSelectorHeight / reductionRatio2, 1.0f));
			mapSelectorListTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.46f - (mapSelectorHeight + mapSelectorListHeight) / reductionRatio2 / 2, 0.55f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapSelectorListWidth / reductionRatio2, mapSelectorListHeight / reductionRatio2, 1.0f));
			mapSelectorScrollerTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.17f, 0.3f, 0.56f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapSelectorScrollerWidth / reductionRatio2, mapSelectorScrollerHeight / reductionRatio2, 1.0f));
			mapIconTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.72f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapIconWidth / reductionRatio3, mapIconHeight / reductionRatio3, 1.0f));
			selectedTeamTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.45f, 0.657f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(selectedTeamWidth / reductionRatio2, selectedTeamHeight / reductionRatio2, 1.0f));
			teamListTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.01f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(teamListWidth / reductionRatio2, teamListHeight / reductionRatio2, 1.0f));
			createTeamTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.3f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(createTeamWidth / reductionRatio2, createTeamHeight / reductionRatio2, 1.0f));
			weaponOptionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(gameOptionWidth / reductionRatio3, gameOptionHeight / reductionRatio3, 1.0f));
			gameOptionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.37f, 0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(gameOptionWidth / reductionRatio3, gameOptionHeight / reductionRatio3, 1.0f));
			schemeSelectorTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.66f, -0.16f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(schemeSelectorWidth / reductionRatio2, schemeSelectorHeight / reductionRatio2, 1.0f));
			deleteTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.972f, -0.28f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(deleteWidth / reductionRatio2, deleteHeight / reductionRatio2, 1.0f));
			saveTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.348f, -0.28f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(deleteWidth / reductionRatio2, deleteHeight / reductionRatio2, 1.0f));
			startTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.755f, -0.54f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(startWidth / reductionRatio2, startHeight / reductionRatio2, 1.0f));
			exitTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.978f, -0.79f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(exitWidth / reductionRatio2, exitHeight / reductionRatio2, 1.0f));
			optionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.17f, -0.53f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(optionWidth / 420.0f, optionHeight / 420.0f, 1.0f));
			cursorTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.7f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.0888888f, 0.05f, 1.0f));

			MapSelectorRect.Set(mapSelectorTransform);
			SelectedTeamRect.Set(selectedTeamTransform);
			TeamListRect.Set(teamListTransform);
			CreateTeamRect.Set(createTeamTransform);
			WeaponsOptionRect.Set(weaponOptionTransform);
			GameOptionRect.Set(gameOptionTransform);
			SchemeSelectorRect.Set(schemeSelectorTransform);
			DeleteRect.Set(deleteTransform);
			SaveRect.Set(saveTransform);
			StartRect.Set(startTransform);
			ExitRect.Set(exitTransform);
			MapSelectorListRect.Set(mapSelectorListTransform);
			MapSelectorScrollerRect.Set(mapSelectorScrollerTransform);
			TeamListNameRect.ResetPos(0.51f, 0.177f, 0.41f, 0.034f);
			TeamListPointsRect.ResetPos(1.02f, 0.177f, 0.105f, 0.034f);

			MapSelectorLayer = new MapListLayer(&mapSelectorScrollerTransform, &MapSelectorScrollerRect, &MouseOnMapList, mapSelectorListTransform[1][1], mapSelectorListTransform[0][0], &mapSelectedIndex);
			m_LayerStack.PushLayer(MapSelectorLayer);

			OptionsRect.resize(BasicOption::OptionMax);
			for (int i = 0; i < BasicOption::OptionMax; ++i)
			{
				OptionsRect[i].ResetPos(-1.17f + OptionsOffset * i, -0.54f, optionWidth / 420.0f / 2, optionHeight / 420.0f / 2);
			}

			for (int i = 0; i < SceneBackground::MaxStar; ++i)
			{
				starInfos.push_back(SceneBackground::StarInfo());
			}
		}

		void CursorUpdate();
		void DrawButtons();
		void DrawFont();
		void ButtonUpdate();
		void MouseButtonClick();

		virtual void Init(const std::any& data) override
		{
			virtualCursorPos = { 640.0f, 360.0f };
			cursorTransform[3][0] = 0.0f;
			cursorTransform[3][1] = 0.0f;

			OptionsIndex[BasicOption::TurnTime] = BasicOption::TurnTime::TT45;
			OptionsIndex[BasicOption::RoundTime] = BasicOption::RoundTime::RT15;
			OptionsIndex[BasicOption::WinRequires] = BasicOption::WinRequires::WR1;
			OptionsIndex[BasicOption::WormEnergy] = BasicOption::WormEnergy::WE150;
			OptionsIndex[BasicOption::WormSelect] = BasicOption::WormSelect::WSOff;
			OptionsIndex[BasicOption::Teleportin] = BasicOption::Teleportin::Off;

			OnMapSelectorActive = false;
		}

		virtual void Update(Gear::Timestep ts) override
		{
			Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			Gear::RenderCommand::Clear();

			Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);

			for (int i = 0; i < SceneBackground::MaxStar; ++i)
			{
				starInfos[i].Update(ts);
				Gear::Renderer2D::DrawFrameQuad(starInfos[i].Transform, Star, 0, starInfos[i].Index);
			}
			Gear::Renderer2D::DrawQuad(gradTransform, Grad);
			Gear::Renderer2D::DrawQuad(TerrainBorderTransform, DivideBorder);
			Gear::Renderer2D::DrawQuad(TeamBorderTransform, DivideBorder);
			Gear::Renderer2D::DrawQuad(SchemeBorderTransform, DivideBorder2);
			Gear::Renderer2D::DrawQuad(BarrackBorderTransform, DivideBorder2);

			CursorUpdate();
			ButtonUpdate();
			DrawButtons();
			DrawFont();

			if (!readyMouseClick)
			{
				mouseClickPastTime += ts;
				if (mouseClickPastTime > MouseClickDeleay)
				{
					mouseClickPastTime = 0.0f;
					readyMouseClick = true;
				}
			}

			if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT) && readyMouseClick)
			{
				readyMouseClick = false;
				if (OnMapSelectorActive && (!MouseOn[Main_Multi::MapSelectorList] && !MouseOn[Main_Multi::MapSelector]))
				{
					OnMapSelectorActive = false;
				}
				if (MouseOn.any())
				{
					MouseButtonClick();
				}
			}

			if (Gear::Input::IsKeyPressd(GR_KEY_ENTER) || Gear::Input::IsKeyPressd(GR_KEY_ESCAPE))
			{
				OnMapSelectorActive = false;
			}

			if (OnMapSelectorActive)
			{
				MapSelectorLayer->OnUpdate(ts);
			}

			Gear::Renderer2D::DrawFrameQuad(cursorTransform, Cursor, 0, 0);
			Gear::Renderer2D::EndScene();
		}

		void ChangeTurnTime();
		void ChangeRoundTime();
		void ChangeWinRequires();
		void ChangeWormSelect();
		void ChangeWormEenergy();
		void ChangeTeleportin();
		void OnMapSelector();
		void MapChange();
		void OnTeams();
		void OnWeaponsSet();
		void OnGameOptionSet();
		void OnSchemeSelector();
		void OnDelete();
		void OnSave();
		void OnBarracks();
		void OnTeamName();
		void OnPoints();
		void OnCreateNewTeam();
		void StartGame();
		void OnExit();
	};




}