#include "wmpch.h"
#include "IntroScene.h"

namespace Intro {

	bool IntroScene::loadingComplete = false;
	std::vector<Gear::TextureData> textureDatas;

	void ResourceLoading()
	{
		std::string dir = std::experimental::filesystem::current_path().string();
		dir += "/assets/textures";

		for (auto& p : std::experimental::filesystem::recursive_directory_iterator(dir))
		{
			std::string path = p.path().string();
			int length = path.length();
			if (path[length - 4] == '.')
			{
				textureDatas.emplace_back(Gear::TextureData(path));
			}
		}
		IntroScene::loadingComplete = true;
	}


	void IntroScene::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);

		if (!inFirst && !isEndScene && resourceLoadGpu)
		{
			if (Gear::Input::IsKeyPressd(GR_KEY_ENTER) || Gear::Input::IsKeyPressd(GR_KEY_ESCAPE) || Gear::Input::IsKeyPressd(GR_KEY_SPACE)
				|| Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT) || Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
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
		else if (!inFirst && resourceLoadGpu)
		{
			Gear::Renderer2D::DrawQuad(introTranslate, introTexture);
			pastTime += ts;
			if (pastTime >= sceneChangeDelay)
			{
				isEndScene = true;
			}
		}

		if (loadingComplete && !resourceLoadGpu)
		{
			resourceLoadGpu = true;
			for (int i = 0; i < textureDatas.size(); ++i)
			{
				if (textureDatas[i].isFrameData)
				{
					Gear::TextureStorage::AddFrameTexture2D(textureDatas[i].Name, Gear::FrameTexture2D::Create(textureDatas[i]));
				}
				else
				{
					Gear::TextureStorage::AddTexture2D(textureDatas[i].Name, Gear::Texture2D::Create(textureDatas[i]));
				}
			}
			//animation
			std::vector<std::pair<int, int>> orderVector;
			for (int i = 0; i < 32; ++i)
			{
				if (i < 16)
				{
					orderVector.push_back({ 0, i });
				}
				else
				{
					orderVector.push_back({ 0, 31 - i });
				}
			}
			auto redArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RedFollowingArrow"), 0.02f, orderVector, true);
			auto blueArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("BlueFollowingArrow"), 0.02f, orderVector, true);
			redArrow->Start();
			blueArrow->Start();
			Gear::TextureStorage::AddAnimation("RedFollowArrow", redArrow);
			Gear::TextureStorage::AddAnimation("BlueFollowArrow", blueArrow);

			auto redCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RedCursor"), 0.0f, true);
			auto blueCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("BlueCursor"), 0.0f, true);
			Gear::TextureStorage::AddAnimation("RedCursor", redCursor);
			Gear::TextureStorage::AddAnimation("BlueCursor", blueCursor);
		}

		Gear::Renderer2D::EndScene();
	}
}
