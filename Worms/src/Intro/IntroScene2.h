#pragma once

namespace Intro {

	void ResourceLoading();

	class IntroScene2 : public Gear::Scene
	{
	private:
		Gear::Ref<Gear::Texture2D> titleTexture;
		Gear::Ref<Gear::Texture2D> backTexture;
		Gear::OrthographicCamera introCamera;

		Gear::Ref<Gear::Texture2D> amargeddonTexture[10];

		std::string amargeddonStr[10] = { "a1", "m", "a2", "r", "g", "e", "d1", "d2", "o", "n" };
		glm::mat4 titleTransform;
		glm::mat4 backTransform;
		glm::mat4 amargeddonTransform;

	public:
		bool inFirst = true;
		bool isEndScene = false;
		glm::vec4 alpha;
		float pastTime = 0.0f;
		float sceneChangeDelay = 3.0f;

	public:
		IntroScene2()
			: Scene("IntroScene2"), introCamera(-1.7777f, 1.7777f, -1.0f, 1.0f), alpha(1.0f, 1.0f, 1.0f, 0.0f)
		{
			titleTexture = Gear::TextureStorage::GetTexture2D("IntroTitle");
			backTexture = Gear::TextureStorage::GetTexture2D("IntroBack");

			for (int i = 0; i < 10; ++i)
			{
				amargeddonTexture[i] = Gear::TextureStorage::GetTexture2D(amargeddonStr[i]);
			}

			titleTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.5555555f, 2.0f, 1.0f));
			backTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.5555555f, 2.0f, 1.0f));
			amargeddonTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.5555555f, 2.0f, 1.0f));
		}

		virtual void Update(Gear::Timestep ts) override
		{
			Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			Gear::RenderCommand::Clear();

			Gear::Renderer2D::BeginScene(introCamera);

			Gear::Renderer2D::DrawQuad(titleTransform, titleTexture);
			Gear::Renderer2D::DrawQuad(backTransform, backTexture);

			Gear::Renderer2D::EndScene();
		}

	};
}


