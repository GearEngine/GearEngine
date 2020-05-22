#pragma once

#include "Main/MainScene.h"

namespace Intro {

	class IntroScene2 : public Gear::Scene
	{
	private:
		Gear::Ref<Gear::Texture2D> titleTexture;
		Gear::Ref<Gear::Texture2D> backTexture;

		Gear::Ref<Gear::Texture2D> amargeddonTexture[10];

		std::string amargeddonStr[10] = { "a1", "m", "a2", "r", "g", "e", "d1", "d2", "o", "n" };
		glm::mat4 titleTransform;
		glm::mat4 backTransform;
		glm::mat4 amargeddonTransform;

	public:
		bool inFirst = true;
		bool isEndScene = false;
		glm::vec4 alpha;
		

	public:
		IntroScene2()
			: Scene("IntroScene2"), alpha(1.0f, 1.0f, 1.0f, 1.0f)
		{
			titleTexture = Gear::TextureStorage::GetTexture2D("IntroTitle");
			backTexture = Gear::TextureStorage::GetTexture2D("IntroBack");

			for (int i = 0; i < 10; ++i)
			{
				amargeddonTexture[i] = Gear::TextureStorage::GetTexture2D(amargeddonStr[i]);
			}

			int tiltleWidth = titleTexture->GetWidth();
			int tiltleHeight = titleTexture->GetHeight();
			int backWidth = backTexture->GetWidth();
			int backHeight = backTexture->GetHeight();
			int amargeddonWidth = amargeddonTexture[0]->GetWidth();
			int amargeddonHeight = amargeddonTexture[0]->GetHeight();

			float reductionRatio = 238.0f;

			titleTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.65f, 0.1f)) * glm::scale(glm::mat4(1.0f), glm::vec3(tiltleWidth / reductionRatio, tiltleHeight / reductionRatio, 1.0f));
			backTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(backWidth / reductionRatio, backHeight / reductionRatio, 1.0f));
			amargeddonTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2f, 0.4f, 0.2f)) * glm::scale(glm::mat4(1.0f), glm::vec3(amargeddonWidth / reductionRatio, amargeddonHeight / reductionRatio, 1.0f));
		}

		virtual void Update(Gear::Timestep ts) override;

	};
}


