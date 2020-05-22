#include "wmpch.h"
#include "IntroScene.h"

namespace Intro {

	bool IntroScene::loadingTextureComplete = false;
	bool IntroScene::loadingSoundComplete = false;
	std::vector<Gear::TextureData> textureDatas;

	void TextureLoading()
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
		IntroScene::loadingTextureComplete = true;
	}

	std::string GetName(const std::string& path)
	{
		std::string ret;
		int length = path.length();
		int nameStartPoint;
		for (int i = length - 1; i > 0; --i)
		{
			if (path[i] == '\\' || path[i] == '/')
			{
				nameStartPoint = i + 1;
				break;
			}
		}
		int nameLength = length - nameStartPoint - 4;
		ret = path.substr(nameStartPoint, nameLength);
		return ret;
	}

	void SoundLoading()
	{
		std::string dir = std::experimental::filesystem::current_path().string();
		dir += "/assets/Sound";

		for (auto& p : std::experimental::filesystem::recursive_directory_iterator(dir))
		{
			std::string path = p.path().string();
			int length = path.length();
			if (std::experimental::filesystem::is_regular_file(path))
			{
				if (path.substr(length - 10) == "Stream.wav")
				{
					ADD_SOUND(path, GetName(path), true, true);
				}
				else
				{
					ADD_SOUND(path, GetName(path), false, false);
				}
			}
		}
		IntroScene::loadingSoundComplete = true;
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

		if (loadingTextureComplete && !resourceLoadGpu)
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
			auto GreenArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("GreenFollowingArrow"), 0.02f, orderVector, true);
			auto YellowArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("YellowFollowingArrow"), 0.02f, orderVector, true);
			auto SkyArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("SkyFollowingArrow"), 0.02f, orderVector, true);
			auto PurpleArrow = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("PurpleFollowingArrow"), 0.02f, orderVector, true);
			
			redArrow->Start();
			blueArrow->Start();
			GreenArrow->Start();
			YellowArrow->Start();
			SkyArrow->Start();
			PurpleArrow->Start();
			Gear::TextureStorage::AddAnimation("RedFollowArrow", redArrow);
			Gear::TextureStorage::AddAnimation("BlueFollowArrow", blueArrow);
			Gear::TextureStorage::AddAnimation("GreenFollowArrow", GreenArrow);
			Gear::TextureStorage::AddAnimation("YellowFollowArrow", YellowArrow);
			Gear::TextureStorage::AddAnimation("SkyFollowArrow", SkyArrow);
			Gear::TextureStorage::AddAnimation("PurpleFollowArrow", PurpleArrow);

			auto redCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("RedCursor"), 0.0f, true);
			auto blueCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("BlueCursor"), 0.0f, true);
			auto greenCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("GreenCursor"), 0.0f, true);
			auto yellowCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("YellowCursor"), 0.0f, true);
			auto skyCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("SkyCursor"), 0.0f, true);
			auto purpleCursor = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("PurpleCursor"), 0.0f, true);
			
			Gear::TextureStorage::AddAnimation("RedCursor", redCursor);
			Gear::TextureStorage::AddAnimation("BlueCursor", blueCursor);
			Gear::TextureStorage::AddAnimation("GreenCursor", greenCursor);
			Gear::TextureStorage::AddAnimation("YellowCursor", yellowCursor);
			Gear::TextureStorage::AddAnimation("SkyCursor", skyCursor);
			Gear::TextureStorage::AddAnimation("PurpleCursor", purpleCursor);
		}

		Gear::Renderer2D::EndScene();
	}
}
