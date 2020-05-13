#pragma once

namespace Main {

	class MainScene : public Gear::Scene
	{
		float reductionRatio = 180.0f;
		float reductionRatio2 = 245.0f;
		float reductionRatio3 = 280.0f;

		bool inFirst = true;
		glm::mat4 alphaTransform;
		glm::vec4 alphaColor;

		Gear::Ref<Gear::Texture2D> Grad;
		glm::mat4 gradTransform;

		Gear::Ref<Gear::Texture2D> Single;
		Gear::Ref<Gear::Texture2D> SingleReady;
		bool mouseOnSingle = false;
		glm::mat4 singleTransform;
		Gear::Util::FRect singleRect;
		
		Gear::Ref<Gear::Texture2D> Multi;
		Gear::Ref<Gear::Texture2D> MultiReady;
		bool mouseOnMulti = false;
		glm::mat4 multiTransform;
		Gear::Util::FRect multiRect;

		Gear::Ref<Gear::Texture2D> Net;
		Gear::Ref<Gear::Texture2D> NetReady;
		bool mouseOnNet = false;
		glm::mat4 netTransform;
		Gear::Util::FRect netRect;
		
		Gear::Ref<Gear::Texture2D> Options;
		Gear::Ref<Gear::Texture2D> OptionsReady;
		bool mouseOnOptions = false;
		glm::mat4 optionsTransform;
		Gear::Util::FRect optionRect;
		
		Gear::Ref<Gear::Texture2D> Quit;
		Gear::Ref<Gear::Texture2D> QuitReady;
		bool mouseOnQuit = false;
		glm::mat4 quitTransform;
		Gear::Util::FRect quitRect;

		Gear::Ref<Gear::Texture2D> Logo;
		glm::mat4 logoTransform;

		Gear::Ref<Gear::FrameTexture2D> Cursor;
		glm::mat4 cursorTransform;

		Gear::Ref<Gear::FrameTexture2D> Star;
		std::vector<SceneBackground::StarInfo> starInfos;

		std::pair<float, float> centerMousePos;
		std::pair<float, float> virtualCursorPos;

	public:
		MainScene()
			: Scene("MainScene"), centerMousePos(640.0f, 360.0f), alphaColor(0.0f, 0.0f, 0.01f, 1.0f)
		{
			Grad = Gear::TextureStorage::GetTexture2D("minigrad");
			Logo = Gear::TextureStorage::GetTexture2D("wgn");
			Cursor = Gear::TextureStorage::GetFrameTexture2D("Cursor");

			Multi = Gear::TextureStorage::GetTexture2D("multi");
			Net = Gear::TextureStorage::GetTexture2D("net");
			Options = Gear::TextureStorage::GetTexture2D("options");
			Single = Gear::TextureStorage::GetTexture2D("single");

			MultiReady = Gear::TextureStorage::GetTexture2D("multiOnReady");
			NetReady = Gear::TextureStorage::GetTexture2D("netOnReady");
			OptionsReady = Gear::TextureStorage::GetTexture2D("optionsOnReady");
			SingleReady = Gear::TextureStorage::GetTexture2D("singleOnReady");
			
			Quit = Gear::TextureStorage::GetTexture2D("Quit");
			QuitReady = Gear::TextureStorage::GetTexture2D("QuitReady");

			Star = Gear::TextureStorage::GetFrameTexture2D("FallenStar");
			for (int i = 0; i < SceneBackground::MaxStar; ++i)
			{
				starInfos.push_back(SceneBackground::StarInfo());
			}

			int gradWidth = Grad->GetWidth();

			int multiWidth = Multi->GetWidth();
			int multiHeight= Multi->GetHeight();

			int logoWidth = Logo->GetWidth();
			int logoHeight = Logo->GetHeight();
			
			int quitWidth = Quit->GetWidth();
			int quitHeight = Quit->GetHeight();

			alphaTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.555555555, 2.0f, 1.0f));
			gradTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(gradWidth / reductionRatio, gradWidth / reductionRatio, 1.0f));
			
			float yOffset = 0.07f;
			singleTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.32f - yOffset, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(multiWidth / reductionRatio2, multiHeight / reductionRatio2, 1.0f));
			singleRect.Set(singleTransform);
			multiTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.32f - yOffset, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(multiWidth / reductionRatio2, multiHeight / reductionRatio2, 1.0f));
			multiRect.Set(multiTransform);
			netTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.32f - yOffset, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(multiWidth / reductionRatio2, multiHeight / reductionRatio2, 1.0f));
			netRect.Set(netTransform);
			optionsTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.32f - yOffset, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(multiWidth / reductionRatio2, multiHeight / reductionRatio2, 1.0f));
			optionRect.Set(optionsTransform);
			quitTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.71f, -0.8f, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(quitWidth / reductionRatio2, quitHeight / reductionRatio2, 1.0f));
			quitRect.Set(quitTransform);
			cursorTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.0888888f, 0.05f, 1.0f));

			logoTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.8f, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(logoWidth / reductionRatio2, logoHeight / reductionRatio2, 1.0f));
		}

		virtual void Update(Gear::Timestep ts) override;

		virtual void Init(const std::any& data) override
		{
			cursorTransform[3][0] = 0.0f;
			cursorTransform[3][1] = 0.0f;
			virtualCursorPos = { 640.0f, 360.0f };
		}
	};
}