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
		GR_TRACE("IngameScene loading Complete");
	}

	void LoadingScene::Init(const std::any& data)
	{
		InitData = std::any_cast<InGame::InitiateData>(data);
		loadingThread = std::thread(loadInGameScene);
		loadingThread.detach();
		loadComplete = false;
		pastTime = 0.0f;
		index = 0;
	}

	void LoadingScene::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(loadCamara);

		if (index < maxIndex)
		{
			pastTime += ts;
			if (pastTime >= FrameDelay)
			{
				pastTime = 0.0f;
				++index;
			}
		}
		Gear::Renderer2D::DrawFrameQuad(loadingTransform, Loading, 0, index);
		Gear::Renderer2D::EndScene();

		if (index == maxIndex && loadComplete)
		{
			Gear::SceneManager::Get()->AddScene(inGameScene);
			Gear::SceneManager::Get()->changeScene("InGame");
			Gear::Application::Get().ActivateEntitySystem(true);
		}
	}

}