#pragma once

namespace Loading {

	class LoadingScene : public Gear::Scene
	{
		Gear::Ref<Gear::FrameTexture2D> Loading;
		glm::mat4 loadingTransform;
		std::thread loadingThread;

		float pastTime = 0.0f;
		float FrameDelay = 0.1f;
		int index = 0;
		int maxIndex = 25;

	public:
		LoadingScene()
			: Scene("LoadingScene")
		{
			Loading = Gear::TextureStorage::GetFrameTexture2D("Loading");
			loadingTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3444f, 0.2f, 1.0f));
		}

		virtual void Init(const std::any& data) override;
		virtual void Update(Gear::Timestep ts) override;
	};

}