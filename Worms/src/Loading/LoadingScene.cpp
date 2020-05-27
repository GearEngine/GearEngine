#include "wmpch.h"
#include "LoadingScene.h"

#include "InGame/InGameScene.h"

namespace Loading {

	InGame::InGameScene* inGameScene = nullptr;
	InGame::InitiateData InitData;
	bool loadComplete = false;

	void loadInGameScene()
	{
		inGameScene = new InGame::InGameScene(InitData);
		loadComplete = true;
		GR_INFO("IngameScene loading Complete");
	}

	void LoadingScene::Init(const std::any& data)
	{
		InitData = std::any_cast<InGame::InitiateData>(data);
		loadComplete = false;
		loadingThread = std::thread(loadInGameScene);
		loadingThread.detach();
		pastTime = 0.0f;
		index = maxIndex - 1;
	}

	void LoadingScene::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);

		if (index > 0)
		{
			pastTime += ts;
			if (pastTime >= FrameDelay)
			{
				PLAY_SOUND_NAME("LoadingTick", WormsSound::effect);
				pastTime = 0.0f;
				--index;
			}
		}
		Gear::Renderer2D::DrawFrameQuad(loadingTransform, Loading, 0, index);
		Gear::Renderer2D::EndScene();

		if (index == 0 && loadComplete)
		{
			Gear::SceneManager::Get()->AddScene(inGameScene);
			Gear::SceneManager::Get()->changeScene("InGame");
			Gear::Application::Get().ActivateEntitySystem(true);
		}
	}

}