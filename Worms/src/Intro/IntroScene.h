#pragma once

#include "IntroScene2.h"

namespace Intro {

	void TextureLoading();
	void SoundLoading();

	class IntroScene : public Gear::Scene
	{
	private:
		std::thread textureLoadingThread;
		std::thread soundLoadingThread;

		glm::mat4 introTranslate;
		Gear::Ref<Gear::Texture2D> introTexture;

	public:
		static bool loadingTextureComplete;
		static bool loadingSoundComplete;

		bool inFirst = true;
		bool isEndScene = false;
		glm::vec4 alpha;
		float pastTime = 0.0f;
		float sceneChangeDelay = 3.0f;
		bool resourceLoadGpu = false;
		bool animationCreate = false;

		int loadIndex = 0;
		const int perFramLoadMax = 200;

	public:
		IntroScene()
			: Scene("IntroScene"), alpha(1.0f, 1.0f, 1.0f, 0.0f)
		{
			Gear::TextureStorage::AddTexture2D("Intro", Gear::Texture2D::Create("assets/textures/Intro/TEAM17.png"));

			textureLoadingThread = std::thread(TextureLoading);
			soundLoadingThread = std::thread(SoundLoading);
			textureLoadingThread.detach();
			soundLoadingThread.detach();

			introTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.8f, 1.5f, 1.0f));
			introTexture = Gear::TextureStorage::GetTexture2D("Intro");
		}

		virtual void Update(Gear::Timestep ts) override;
	};
}