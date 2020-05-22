#include "wmpch.h"
#include "IntroScene2.h"

#include "IntroScene.h"

namespace Intro {

	void IntroScene2::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);

		if (isEndScene)
		{
			alpha.a -= 0.02f;
			Gear::Renderer2D::DrawQuad(titleTransform, titleTexture, alpha);
			Gear::Renderer2D::DrawQuad(backTransform, backTexture, alpha);
			if (alpha.a <= 0.0f)
			{
				Gear::SceneManager::Get()->AddScene(new Main::MainScene);
				Gear::SceneManager::Get()->changeScene("MainScene");
			}
		}
		else
		{
			Gear::Renderer2D::DrawQuad(titleTransform, titleTexture);
			Gear::Renderer2D::DrawQuad(backTransform, backTexture);
		}

		if (IntroScene::loadingSoundComplete)
		{
			if (Gear::Input::IsKeyPressd(GR_KEY_ENTER) || Gear::Input::IsKeyPressd(GR_KEY_ESCAPE) || Gear::Input::IsKeyPressd(GR_KEY_SPACE)
				|| Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_RIGHT) || Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
			{
				isEndScene = true;
			}
		}


		Gear::Renderer2D::EndScene();
	}
}