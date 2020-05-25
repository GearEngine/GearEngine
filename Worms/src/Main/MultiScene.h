#pragma once

namespace Main {

	class BasicTeamInfo : public Gear::JsonAble
	{
	public:
		std::string teamName;
		std::string teamIcon;
		unsigned int playerType;
		int points;
		std::vector<std::string> wormName;
		std::vector<unsigned int> graves;

		BasicTeamInfo(const std::string& name = "")
			: Gear::JsonAble(name)
		{
			wormName.resize(8);
			graves.resize(8);
		}


		virtual void Read(const Json::Value& value) override
		{
			teamName = value["teamName"].asString();
			teamIcon = value["teamIcon"].asString();
			points = value["points"].asInt();
			playerType = value["playerType"].asUInt();

			wormName.resize(8);
			for (int i = 0; i < 8; ++i)
			{
				wormName[i] = value["wormName"][i].asString();
			}
			for (int i = 0; i < 8; ++i)
			{
				graves[i] = value["graves"][i].asUInt();
			}
		}
		virtual void Write(Json::Value& value) override
		{
			value["teamName"] = teamName;
			value["teamIcon"] = teamIcon;
			value["points"] = points;
			value["playerType"] = playerType;

			for (int i = 0; i < 8; ++i)
			{
				value["wormName"][i] = wormName[i];
				value["graves"][i] = graves[i];
			}
		}
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

	class SchemeListLayer : public Gear::Layer
	{
	public:
		SchemeListLayer(glm::mat4 schemeSelectorTransform, glm::mat4* _schemeListTransform, bool* OnSchemeSelectorActivate)
			: scrollerColor(254.0f / 255.0f, 182.0f / 255.0f, 168.0f / 255.0f, 1.0f)
		{
			onActivate = OnSchemeSelectorActivate;

			LoadSchemeDataName();
			curScheme = schemeList[highlighterIndex];

			schemeListTexture = Gear::TextureStorage::GetTexture2D("SchemeSelectorList");
			float listWidth = schemeListTexture->GetWidth();
			float listHeight = schemeListTexture->GetHeight();
			schemeListTransform = glm::translate(glm::mat4(1.0f), glm::vec3(schemeSelectorTransform[3][0], schemeSelectorTransform[3][1] - schemeSelectorTransform[1][1] / 2 - listHeight / 238.0f / 2.0f, 0.55f)) * glm::scale(glm::mat4(1.0f), glm::vec3(listWidth / 238.0f, listHeight / 238.0f, 1.0f));
			schemeListRect.Set(schemeListTransform);
			*_schemeListTransform = schemeListTransform;

			listFieldTop = schemeListTransform[1][1] / 2 + schemeListTransform[3][1] - schemeSelectorTransform[1][1] / 2;
			MapSelectorDivideUnit = ((listHeight / 238.0f) - 0.02f) / showMapNameMax;
			fieldTransforms.resize(showMapNameMax);
			fieldRects.resize(showMapNameMax);
			for (int i = 0; i < showMapNameMax; ++i)
			{
				fieldTransforms[i] = glm::translate(glm::mat4(1.0f), glm::vec3(fieldCenterX, listFieldTop - MapSelectorDivideUnit * i, 0.57f)) * glm::scale(glm::mat4(1.0f), glm::vec3(schemeSelectorTransform[0][0] - 0.07f, MapSelectorDivideUnit, 1.0f));
				fieldRects[i].Set(fieldTransforms[i]);
			}

			ScrollerTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.118f, 0.0f, 0.6f)) * glm::scale(glm::mat4(1.0f), glm::vec3(8 / 238.0f, 0.0f, 1.0f));
			ScrollerTop = schemeListTransform[3][1] + schemeListTransform[1][1] / 2 - 0.055f;
			ScrollerBottom = schemeListTransform[3][1] - schemeListTransform[1][1] / 2 + 0.055;

			if (maxListShowIndex < 0)
			{
				maxListShowIndex = 0;
			}

			RecalculateScrollerPos();
			ScrollerRect.Set(ScrollerTransform);
		}

	public:
		std::vector<std::string> schemeList;
		std::string curScheme;

		Gear::Ref<Gear::Texture2D> schemeListTexture;
		Gear::Util::FRect schemeListRect;
		glm::mat4 schemeListTransform;

		//
		Gear::Ref<Gear::Texture2D> Scroller;
		glm::vec4 scrollerColor;
		Gear::Util::FRect ScrollerRect;
		glm::mat4 ScrollerTransform;

		float ScrollerTop;
		float ScrollerBottom;
		float scrollerHighestYpos;
		float scrollerLowestYpos;
		float scrollerYOffset;

		float listFieldTop;
		float fieldCenterX = -0.6848f;
		float fieldFront = -1.17f;

		int highlighterIndex = 0;
		int curListShowIndex = 0;
		int maxListShowIndex;

		int listIndexMax;
		const int showMapNameMax = 12;


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

		bool* onActivate;
		bool MouseOnListrect = false;
		int* SelectedIndex;

	public:
		void RecalculateScrollerPos();

		int GetShowIndex(int curIndex)
		{
			return curIndex - curListShowIndex;
		}

		int GetListIndex(int showIndex)
		{
			return showIndex + curListShowIndex;
		}

		void LoadSchemeDataName();
		void calcListShowIndex();
		void OnReinit();
		virtual void OnUpdate(Gear::Timestep ts) override;

		virtual void OnEvent(Gear::Event& e) override;

		bool OnMouseScrolled(Gear::MouseScrolledEvent & e);

		void KeyInputLogic();
		void MousePressLogic();

	};

	class BarracksLayer : public Gear::Layer 
	{
		std::vector<BasicTeamInfo*> teamInfolist;
		std::vector<Gear::Ref<Gear::Texture2D>> playerTypeIcons;
		std::vector<Gear::Util::FRect> filedRects;
		Gear::Util::FRect BorderRect;

		float SelectorListBorderTop;
		float SelectorListBorderBottom;

		float filedYOffset;
		const int listShowMax = 6;

		int curListShowIndex = 0;
		int listShowIndexMax;

		glm::vec4 scrollerColor;
		float scrollerHighestYpos;
		float scrollerLowestYpos;
		float scrollerYOffset;
		float ScrollerTop;
		float ScrollerBottom;

		glm::mat4 teamNameFiledTranform;
		glm::mat4 playerTypeIconTransform;

		//scroller
		glm::mat4 scrollerTransform;


	public:
		BarracksLayer(const Gear::Util::FRect& borderRect)
			: Gear::Layer("BarraksLayer"), BorderRect(borderRect)
		{
			teamInfolist = Gear::JsonManager::Get()->ReadAll<BasicTeamInfo>("assets\\Data\\TeamInfo");
			scrollerColor = glm::vec4(254.0f / 255.0f, 182.0f / 255.0f, 168.0f / 255.0f, 1.0f);

			sortTeamInfoListName();
			float BorderHeight = BorderRect.Height - 0.034f * 2;
			float BorderWidth = BorderRect.Width - 0.05f;
			SelectorListBorderTop = BorderRect.CenterY - 0.034f + BorderHeight / 2;
			SelectorListBorderBottom = BorderRect.CenterY - 0.034f - BorderHeight / 2;
			filedYOffset = BorderHeight / listShowMax;

			float scrollerWidth = Gear::TextureStorage::GetTexture2D("MapListScroller")->GetWidth();

			playerTypeIcons.resize(PlayerType::Max);
			for (int i = 0; i < PlayerType::Max; ++i)
			{
				playerTypeIcons[i] = Gear::TextureStorage::GetTexture2D(PlayerType::GetName(i));
			}
			
			playerTypeIconTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.17f, SelectorListBorderTop - filedYOffset / 2, 0.4f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.06f, 0.06f, 1.0f));
			teamNameFiledTranform = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, SelectorListBorderTop - filedYOffset / 2, 0.4f)) * glm::scale(glm::mat4(1.0f), glm::vec3(BorderWidth, filedYOffset, 1.0f));
			scrollerTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1.17f, 0.0f, 0.4f)) * glm::scale(glm::mat4(1.0f), glm::vec3(scrollerWidth / 238.0f, 0.0f, 1.0f));
			ScrollerTop = SelectorListBorderTop - scrollerTransform[0][0] * 1.7f;
			ScrollerBottom = SelectorListBorderBottom + scrollerTransform[0][0] * 1.7f;

			filedRects.resize(listShowMax);
			glm::mat4 temp = teamNameFiledTranform;
			for (int i = 0; i < listShowMax; ++i)
			{
				if (i)
				{
					temp[3][1] -= filedYOffset;
				}
				filedRects[i].Set(temp);
			}

			RecalculateScrollerPos();
		}

		void pushTeamData(BasicTeamInfo* teamInfo) 
		{
			teamInfolist.push_back(teamInfo);
			RecalculateScrollerPos();
		}

		void sortTeamInfoListPoint();
		void sortTeamInfoListName();

		void DrawFont()
		{
			glm::mat4 tempFiledTrasform = teamNameFiledTranform;

			int indexMax = listShowMax < teamInfolist.size() ? listShowMax : teamInfolist.size();

			for (int i = 0; i < indexMax; ++i)
			{
				if (i)
				{
					tempFiledTrasform[3][1] -= filedYOffset;
				}
				std::string teamName = teamInfolist[i + curListShowIndex]->teamName;
				int length = teamName.length();

				Font::PrintFont(glm::vec3(tempFiledTrasform[3][0] - 0.03f + length * 0.5f * 0.024f, tempFiledTrasform[3][1], 0.5f), glm::vec3(0.05f, 0.05f, 1.0f), teamName, FontType::WhiteTinySmall, 0.025f, false);
			}

			tempFiledTrasform = teamNameFiledTranform;
			tempFiledTrasform[3][0] += 0.74f;
			for (int i = 0; i < indexMax; ++i)
			{
				if (i)
				{
					tempFiledTrasform[3][1] -= filedYOffset;
				}
				Font::PrintFont(glm::vec3(tempFiledTrasform[3][0], tempFiledTrasform[3][1], 0.5f), glm::vec3(0.05f, 0.05f, 1.0f), std::to_string(teamInfolist[i + curListShowIndex]->points), FontType::WhiteTinySmall, 0.02f, false);
			}

		}

		void RecalculateScrollerPos()
		{
			int listSize = teamInfolist.size();
			if (listSize < listShowMax)
			{
				listSize = 6;
			}
			listShowIndexMax = listSize - listShowMax;

			//showIndex
			if (listSize > 6)
			{
				if (curListShowIndex > listShowIndexMax)
				{
					curListShowIndex = listShowIndexMax;
				}
			}
			else
			{
				curListShowIndex = 0;
			}

			//height
			float scrollerHeight = listShowMax / (float)listSize * (ScrollerTop - ScrollerBottom);
			scrollerTransform[1][1] = scrollerHeight;

			//yPos
			scrollerHighestYpos = ScrollerTop - scrollerHeight / 2;
			scrollerLowestYpos = ScrollerBottom + scrollerHeight / 2;

			float scrollerMaxDist = scrollerHighestYpos - scrollerLowestYpos;

			if (listShowIndexMax > 0)
			{
				scrollerYOffset = scrollerMaxDist / (float)listShowIndexMax;
			}
			else
			{
				scrollerYOffset = 0.0f;
			}

			scrollerTransform[3][1] = scrollerHighestYpos - scrollerYOffset * curListShowIndex;

		}

		void DrawIcon()
		{
			glm::mat4 tempPlayerTypeTransform = playerTypeIconTransform;
			int indexMax = listShowMax < teamInfolist.size() ? listShowMax : teamInfolist.size();

			for (int i = 0; i < indexMax; ++i)
			{
				if (i)
				{
					tempPlayerTypeTransform[3][1] -= filedYOffset;
				}
				Gear::Renderer2D::DrawQuad(tempPlayerTypeTransform, playerTypeIcons[teamInfolist[i + curListShowIndex]->playerType]);
			}
		}

		virtual void OnUpdate(Gear::Timestep ts) override
		{
			DrawFont();
			DrawIcon();
			Gear::Renderer2D::DrawQuad(scrollerTransform, scrollerColor);
		}

		virtual void OnEvent(Gear::Event& e) override;
		bool OnMouseScrolled(Gear::MouseScrolledEvent & e);
		bool OnMouseClick(Gear::MouseButtonPressedEvent& e);
	};

	class SelectedTeamLayer : public Gear::Layer
	{
		BarracksLayer* barracksLayer;

		Gear::Util::FRect teamListBorderRect;
		std::vector<Gear::Util::FRect> filedRects;
		glm::mat4 filedTransform;
		glm::mat4 playerTypeIconTransform;
		const int teamListMax = 6;
		float filedYOffset;
		std::vector<Gear::Ref<Gear::Texture2D>> playerTypeIcons;

		//button
		//Handicap
		std::vector<Gear::Ref<Gear::Texture2D>> handicapTexture;
		std::vector<Gear::Ref<Gear::Texture2D>> handicapReadyTexture;
		std::vector<int> handicapType;
		glm::mat4 handicapTransform;
		std::vector<Gear::Util::FRect> handicapRect;
		std::bitset<6> mouseOnHandicap;

		//Ally
		std::vector<Gear::Ref<Gear::Texture2D>> allyTexture;
		std::vector<Gear::Ref<Gear::Texture2D>> allyReadyTexture;
		std::vector<int> allyType;
		glm::mat4 allyTransform;
		std::vector<Gear::Util::FRect> allyRect;
		std::bitset<6> mouseOnAlly;

		//wormCount
		std::vector<Gear::Ref<Gear::Texture2D>> wormCountTexture;
		std::vector<Gear::Ref<Gear::Texture2D>> wormCountReadyTexture;
		std::vector<int> wormCount;
		glm::mat4 wormCountTransform;
		std::vector<Gear::Util::FRect> wormCountRect;
		std::bitset<6> mouseOnWormCount;

	public:
		SelectedTeamLayer(const Gear::Util::FRect& listBorderRect, BarracksLayer* _barracksLayer)
			: Gear::Layer("SelectedTeamLayer")
		{
			playerTypeIcons.resize(6);
			barracksLayer = _barracksLayer;

			for (int i = 0; i < PlayerType::Max; ++i)
			{
				playerTypeIcons[i] = Gear::TextureStorage::GetTexture2D(PlayerType::GetName(i));
			}

			teamListBorderRect = listBorderRect;
			filedYOffset = teamListBorderRect.Height / teamListMax;

			filedTransform = glm::translate(glm::mat4(1.0f), glm::vec3(teamListBorderRect.CenterX, teamListBorderRect.CenterY + filedYOffset * 2.5f, 0.4f));
			filedTransform[0][0] = teamListBorderRect.Width;
			filedTransform[1][1] = filedYOffset;

			playerTypeIconTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.162f, filedTransform[3][1], 0.4f));
			playerTypeIconTransform[0][0] = 0.06f;
			playerTypeIconTransform[1][1] = 0.06f;

			glm::mat4 temp = filedTransform;
			filedRects.resize(teamListMax);
			for (int i = 0; i < teamListMax; ++i)
			{
				if (i)
				{
					temp[3][1] -= filedYOffset;
				}
				filedRects[i].Set(temp);
			}


			//Button
			//Handicap
			handicapTexture.resize(TeamBasicOption::Handicap::HMax);
			handicapReadyTexture.resize(TeamBasicOption::Handicap::HMax);
			for (int i = 0; i < TeamBasicOption::Handicap::HMax; ++i)
			{
				std::string name = "Handicap" + TeamBasicOption::GetHandicapName(i);
				handicapTexture[i] = Gear::TextureStorage::GetTexture2D(name);
				handicapReadyTexture[i] = Gear::TextureStorage::GetTexture2D(name + "r");
			}
			handicapTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.86f, filedTransform[3][1], 0.4f));
			handicapTransform[0][0] = 0.07f;
			handicapTransform[1][1] = 0.07f;

			glm::mat4 tempHandiTransform = handicapTransform;
			handicapRect.resize(teamListMax);
			for (int i = 0; i < teamListMax; ++i)
			{
				if (i)
				{
					tempHandiTransform[3][1] -= filedYOffset;
				}
				handicapRect[i].Set(tempHandiTransform);
			}
			handicapType.resize(teamListMax, TeamBasicOption::Handicap::None);

			//Ally
			allyTexture.resize(TeamBasicOption::Ally::AMax);
			allyReadyTexture.resize(TeamBasicOption::Ally::AMax);
			for (int i = 0; i < TeamBasicOption::Ally::AMax; ++i)
			{
				std::string name = "Ally" + std::to_string(i + 1);
				allyTexture[i] = Gear::TextureStorage::GetTexture2D(name);
				allyReadyTexture[i] = Gear::TextureStorage::GetTexture2D(name + "r");
			}
			allyTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.94f, filedTransform[3][1], 0.4f));
			allyTransform[0][0] = 0.07f;
			allyTransform[1][1] = 0.07f;

			glm::mat4 tempAllyTransform = allyTransform;
			allyRect.resize(teamListMax);
			for (int i = 0; i < teamListMax; ++i)
			{
				if (i)
				{
					tempAllyTransform[3][1] -= filedYOffset;
				}
				allyRect[i].Set(tempAllyTransform);
			}
			allyType.resize(6);
			for (int i = 0; i < 6; ++i)
			{
				allyType[i] = i;
			}

			//WormCount
			wormCountTexture.resize(TeamBasicOption::WormCount::Max);
			wormCountReadyTexture.resize(TeamBasicOption::WormCount::Max);
			for (int i = 0; i < TeamBasicOption::WormCount::Max; ++i)
			{
				std::string name = "WormCount" + std::to_string(i + 1);
				wormCountTexture[i] = Gear::TextureStorage::GetTexture2D(name);
				wormCountReadyTexture[i] = Gear::TextureStorage::GetTexture2D(name + "r");
			}

			float wormCountTextureWidth = wormCountTexture[0]->GetWidth();
			float wormCountTextureHeight = wormCountTexture[0]->GetHeight();

			wormCountTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1.1f, filedTransform[3][1], 0.4f)) * glm::scale(glm::mat4(1.0f), glm::vec3(wormCountTextureWidth / 238.0f, wormCountTextureHeight / 238.0f, 1.0f));
			
			glm::mat4 tempCountTransform = wormCountTransform;
			wormCountRect.resize(teamListMax);
			for (int i = 0; i < teamListMax; ++i)
			{
				if (i)
				{
					tempCountTransform[3][1] -= filedYOffset;
				}
				wormCountRect[i].Set(tempCountTransform);
			}
			
			wormCount.resize(teamListMax, TeamBasicOption::WormCount::_3);

		}

		void DrawFont();
		void DrawIcon();
		void ButtonUpdate();
		void DrawButton();
		int GetAllyCount();

		virtual void OnUpdate(Gear::Timestep ts) override
		{
			DrawFont();
			DrawIcon();
			ButtonUpdate();
			DrawButton();
		}

		virtual void OnEvent(Gear::Event& e) override;
		bool OnMouseClick(Gear::MouseButtonPressedEvent& e);
		void resetBasicInfo();

		friend class MultiScene;
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

		MapListLayer* MapSelectorLayer;
		int mapSelectedIndex;
		//

		//SchemeSelector
		Gear::Ref<Gear::Texture2D> SchemeSelector;
		Gear::Ref<Gear::Texture2D> SchemeSelectorReady;
		Gear::Util::FRect SchemeSelectorRect;
		glm::mat4 schemeSelectorTransform;
		SchemeListLayer* schemeListLayer;
		bool OnSchemeSelectorActivate = false;

		glm::mat4 schemeListTransform;
		Gear::Util::FRect schemeListRect;
		//

		std::vector<Gear::Ref<Gear::Texture2D>> MapIcons;
		glm::mat4 mapIconTransform;

		//selectedTeam
		Gear::Ref<Gear::Texture2D> SelectedTeam;
		Gear::Util::FRect SelectedTeamRect;
		glm::mat4 selectedTeamTransform;
		SelectedTeamLayer* selectedTeamLayer;

	public:
		static std::vector<BasicTeamInfo*> selectedTeamList;
	private:

		//Barrack
		Gear::Ref<Gear::Texture2D> TeamList;
		Gear::Ref<Gear::Texture2D> TeamListName;
		Gear::Ref<Gear::Texture2D> TeamListPoints;
		Gear::Util::FRect TeamListRect;
		Gear::Util::FRect TeamListNameRect;
		Gear::Util::FRect TeamListPointsRect;
		glm::mat4 teamListTransform;
		BarracksLayer* barraksLayer;

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

		int pastMouseOn = -1;
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
			barraksLayer = new BarracksLayer(TeamListRect);
			selectedTeamLayer = new SelectedTeamLayer(SelectedTeamRect, barraksLayer);
			schemeListLayer = new SchemeListLayer(schemeSelectorTransform, &schemeListTransform, &OnSchemeSelectorActivate);
			schemeListRect.Set(schemeListTransform);
			m_LayerStack.PushLayer(MapSelectorLayer);
			m_LayerStack.PushLayer(barraksLayer);
			m_LayerStack.PushLayer(selectedTeamLayer);
			m_LayerStack.PushLayer(schemeListLayer);

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
		bool needClearInGame = false;

		virtual void Init(const std::any& data) override
		{
			if (std::any_cast<int>(data) == -1)
			{
				needClearInGame = true;
			}

			virtualCursorPos = { 640.0f, 360.0f };
			cursorTransform[3][0] = 0.0f;
			cursorTransform[3][1] = 0.0f;

			OptionsIndex[BasicOption::TurnTime] = BasicOption::TurnTime::TT45;
			OptionsIndex[BasicOption::RoundTime] = BasicOption::RoundTime::RT15;
			OptionsIndex[BasicOption::WinRequires] = BasicOption::WinRequires::WR1;
			OptionsIndex[BasicOption::WormEnergy] = BasicOption::WormEnergy::WE150;
			OptionsIndex[BasicOption::WormSelect] = BasicOption::WormSelect::WSOff;
			OptionsIndex[BasicOption::Teleportin] = BasicOption::Teleportin::Off;

			if (selectedTeamList.size())
			{
				for (int i = 0; i < selectedTeamList.size(); ++i)
				{
					barraksLayer->pushTeamData(selectedTeamList[i]);
				}
				barraksLayer->sortTeamInfoListName();
				selectedTeamList.clear();
			}

			OnMapSelectorActive = false;
		}

		virtual void Update(Gear::Timestep ts) override
		{
			Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			Gear::RenderCommand::Clear();

			Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);

			if (needClearInGame)
			{
				Gear::SceneManager::Get()->EraseScene("InGame");
				Gear::EntitySystem::Shutdown();
				needClearInGame = false;
				PLAY_SOUND_NAME("menu-loop_Stream", WormsSound::bgm);
			}

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
				OnSchemeSelectorActivate = false;
			}

			//Layer Update
			if (OnMapSelectorActive)
			{
				MapSelectorLayer->OnUpdate(ts);
			}
			if (OnSchemeSelectorActivate)
			{
				schemeListLayer->OnUpdate(ts);
			}
			if (selectedTeamList.size())
			{
				selectedTeamLayer->OnUpdate(ts);
			}
			barraksLayer->OnUpdate(ts);

			Gear::Renderer2D::DrawFrameQuad(cursorTransform, Cursor, 0, 0);
			Gear::Renderer2D::EndScene();
		}

		bool allyCheck();
		bool SettingInitData();

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