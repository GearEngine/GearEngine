#pragma once

namespace Main {


	class MultiScene : public Gear::Scene
	{
		InGame::InitiateData initData;

		float reductionRatio = 180.0f;
		float reductionRatio2 = 238.0f;
		float reductionRatio3 = 240.0f;

		Gear::Ref<Gear::FrameTexture2D> Cursor;

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

		Gear::Ref<Gear::Texture2D> MapSelector;
		glm::mat4 mapSelectorTransform;

		std::vector<Gear::Ref<Gear::Texture2D>> MapIcons;
		glm::mat4 mapIconTransform;
		unsigned int selectedMap;

		Gear::Ref<Gear::Texture2D> SelectedTeam;
		glm::mat4 selectedTeamTransform;

		Gear::Ref<Gear::Texture2D> TeamList;
		Gear::Ref<Gear::Texture2D> TeamListName;
		Gear::Ref<Gear::Texture2D> TeamListPoints;
		glm::mat4 teamListTransform;

		Gear::Ref<Gear::Texture2D> CreateTeam;
		Gear::Ref<Gear::Texture2D> CreateTeamReady;
		glm::mat4 createTeamTransform;

		Gear::Ref<Gear::Texture2D> WeaponsOption;
		Gear::Ref<Gear::Texture2D> WeaponsOptionReady;
		glm::mat4 weaponOptionTransform;

		Gear::Ref<Gear::Texture2D> GameOption;
		Gear::Ref<Gear::Texture2D> GameOptionReady;
		glm::mat4 gameOptionTransform;

		Gear::Ref<Gear::Texture2D> Delete;
		Gear::Ref<Gear::Texture2D> DeleteReady;
		glm::mat4 deleteTransform;

		Gear::Ref<Gear::Texture2D> Save;
		Gear::Ref<Gear::Texture2D> SaveReady;
		glm::mat4 saveTransform;

		Gear::Ref<Gear::Texture2D> Start;
		Gear::Ref<Gear::Texture2D> StartReady;
		glm::mat4 startTransform;


		Gear::Ref<Gear::Texture2D> Exit;
		Gear::Ref<Gear::Texture2D> ExitReady;
		glm::mat4 exitTransform;


		std::vector<std::vector<Gear::Ref<Gear::Texture2D>>> Options;
		std::vector<std::vector<Gear::Ref<Gear::Texture2D>>> OptionsReady;

		glm::mat4 optionTransform;

	public:
		MultiScene()
			: Scene("MultiScene")
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

			MapIcons.resize(Map::Max);
			for (int i = 0; i < Map::Max; ++i)
			{
				MapIcons[i] = Gear::TextureStorage::GetTexture2D(Map::GetName(i) + "Icon");
			}
			selectedMap = Map::City;

			Options.resize(BasicOption::OptionMax);
			OptionsReady.resize(BasicOption::OptionMax);

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
			float deleteWidth = Delete->GetWidth() - 10.0f;
			float deleteHeight = Delete->GetHeight();
			float exitWidth = Exit->GetWidth() - 10.0f;
			float exitHeight = Exit->GetHeight();
			float startWidth = Start->GetWidth() - 30.0f;
			float startHeight = Start->GetHeight();
			float optionWidth = Options[0][0]->GetWidth();
			float optionHeight = Options[0][0]->GetHeight();


			gradTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(GradWidth / reductionRatio, GradWidth / reductionRatio, 1.0f));
			TerrainBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.65f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorderWidth / reductionRatio2, divideBorderHeight / reductionRatio2, 1.0f));
			TeamBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, 0.65f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorderWidth / reductionRatio2, divideBorderHeight / reductionRatio2, 1.0f));
			SchemeBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, -0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorder2Width / reductionRatio2, divideBorder2Height / reductionRatio2, 1.0f));
			BarrackBorderTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(divideBorder2Width / reductionRatio2, divideBorder2Height / reductionRatio2, 1.0f));
			mapSelectorTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.46f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapSelectorWidth / reductionRatio2, mapSelectorHeight / reductionRatio2, 1.0f));
			mapIconTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.65f, 0.72f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(mapIconWidth / reductionRatio3, mapIconHeight / reductionRatio3, 1.0f));
			selectedTeamTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.45f, 0.657f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(selectedTeamWidth / reductionRatio2, selectedTeamHeight / reductionRatio2, 1.0f));
			teamListTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.01f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(teamListWidth / reductionRatio2, teamListHeight / reductionRatio2, 1.0f));
			createTeamTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.65f, -0.3f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(createTeamWidth / reductionRatio2, createTeamHeight / reductionRatio2, 1.0f));
			weaponOptionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(gameOptionWidth / reductionRatio3, gameOptionHeight / reductionRatio3, 1.0f));
			gameOptionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.37f, 0.05f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(gameOptionWidth / reductionRatio3, gameOptionHeight / reductionRatio3, 1.0f));
			deleteTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.99f, -0.28f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(deleteWidth / reductionRatio2, deleteHeight / reductionRatio2, 1.0f));
			saveTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.335f, -0.28f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(deleteWidth / reductionRatio2, deleteHeight / reductionRatio2, 1.0f));
			startTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.755f, -0.54f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(startWidth / reductionRatio2, startHeight / reductionRatio2, 1.0f));
			exitTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.978f, -0.79f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(exitWidth / reductionRatio2, exitHeight / reductionRatio2, 1.0f));
			optionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.1f, -0.5f, 0.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(optionWidth / reductionRatio3, optionHeight / reductionRatio3, 1.0f));

			for (int i = 0; i < SceneBackground::MaxStar; ++i)
			{
				starInfos.push_back(SceneBackground::StarInfo());
			}

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

			Gear::Renderer2D::DrawQuad(mapSelectorTransform, MapSelector);
			Gear::Renderer2D::DrawQuad(mapIconTransform, MapIcons[selectedMap]);
			Gear::Renderer2D::DrawQuad(selectedTeamTransform, SelectedTeam);
			Gear::Renderer2D::DrawQuad(teamListTransform, TeamList);
			Gear::Renderer2D::DrawQuad(createTeamTransform, CreateTeam);

			Gear::Renderer2D::DrawQuad(weaponOptionTransform, WeaponsOption);
			Gear::Renderer2D::DrawQuad(gameOptionTransform, GameOption);

			Gear::Renderer2D::DrawQuad(deleteTransform, Delete);
			Gear::Renderer2D::DrawQuad(saveTransform, Delete);

			Gear::Renderer2D::DrawQuad(startTransform, Start);
			Gear::Renderer2D::DrawQuad(exitTransform, Exit);

			for (int i = 0; i < BasicOption::OptionMax; ++i)
			{

			}

			Gear::Renderer2D::EndScene();
		}
	};


}