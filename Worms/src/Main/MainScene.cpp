#include "wmpch.h"
#include "MainScene.h"

#include "Loading/LoadingScene.h"
#include "MultiScene.h"

#include "GLFW/include/GLFW/glfw3.h"

namespace Main {

	void MainScene::Update(Gear::Timestep ts)
	{
		Gear::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(SceneBackground::UiCamera);
		Gear::Renderer2D::DrawQuad(gradTransform, Grad);

		if (mouseOnSingle)
		{
			Gear::Renderer2D::DrawQuad(singleTransform, SingleReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(singleTransform, Single);
		}

		if (mouseOnNet)
		{
			Gear::Renderer2D::DrawQuad(netTransform, NetReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(netTransform, Net);
		}

		if (mouseOnMulti)
		{
			Gear::Renderer2D::DrawQuad(multiTransform, MultiReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(multiTransform, Multi);
		}

		if (mouseOnOptions)
		{
			Gear::Renderer2D::DrawQuad(optionsTransform, OptionsReady);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(optionsTransform, Options);
		}

		if (mouseOnQuit)
		{
			Gear::Renderer2D::DrawQuad(quitTransform, QuitReady);
			Font::PrintFont(glm::vec3(0.73f, -0.8f, 0.4f), glm::vec3(0.1f, 0.1f, 1.0f), "Quit", FontType::WhiteSmall, 0.055f, false);
		}
		else
		{
			Gear::Renderer2D::DrawQuad(quitTransform, Quit);
			Font::PrintFont(glm::vec3(0.73f, -0.8f, 0.4f), glm::vec3(0.1f, 0.1f, 1.0f), "Quit", FontType::GraySmall, 0.055f, false);
		}

		Gear::Renderer2D::DrawQuad(logoTransform, Logo);

		auto mousePos = Gear::Input::GetMousePosition();
		if (mousePos != centerMousePos)
		{
			cursorTransform[3][0] += (mousePos.first - centerMousePos.first) * 0.0013f;
			cursorTransform[3][1] += (centerMousePos.second - mousePos.second) * 0.00221f;

			if (cursorTransform[3][0] >= 1.37f)
			{
				cursorTransform[3][0] = 1.37f;
			}
			if (cursorTransform[3][0] <= -1.28f)
			{
				cursorTransform[3][0] = -1.28f;
			}
			if (cursorTransform[3][1] >= 0.98f)
			{
				cursorTransform[3][1] = 0.98f;
			}
			if (cursorTransform[3][1] <= -1.0f)
			{
				cursorTransform[3][1] = -1.0f;
			}
			virtualCursorPos.first = cursorTransform[3][0] - 0.01f;
			virtualCursorPos.second = cursorTransform[3][1] + 0.01f;
		}

		mouseOnSingle = false;
		mouseOnNet = false;
		mouseOnMulti = false;
		mouseOnOptions = false;
		mouseOnQuit = false;
		isOnButton = false;

		if (Gear::Util::IsPointRectCollision(virtualCursorPos, singleRect))
		{
			mouseOnSingle = true;
			isOnButton = true;
			if (prevOnCursorButton != 0)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 0;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, netRect))
		{
			mouseOnNet = true;
			isOnButton = true;
			if (prevOnCursorButton != 1)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 1;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, multiRect))
		{
			mouseOnMulti = true;
			isOnButton = true;
			if (prevOnCursorButton != 2)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 2;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, optionRect))
		{
			mouseOnOptions = true;
			isOnButton = true;
			if (prevOnCursorButton != 3)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 3;
		}
		if (Gear::Util::IsPointRectCollision(virtualCursorPos, quitRect))
		{
			mouseOnQuit = true;
			isOnButton = true;
			if (prevOnCursorButton != 4)
			{
				PLAY_SOUND_NAME("keyerase", WormsSound::effect);
			}
			prevOnCursorButton = 4;
		}

		if (!isOnButton)
		{
			prevOnCursorButton = -1;
		}


		for (int i = 0; i < SceneBackground::MaxStar; ++i)
		{
			starInfos[i].Update(ts);
			Gear::Renderer2D::DrawFrameQuad(starInfos[i].Transform, Star, 0, starInfos[i].Index);
		}

		glfwSetCursorPos((GLFWwindow*)Gear::Application::Get().GetWindow().GetNativeWindow(), 640.0, 360.0);
		Gear::Renderer2D::DrawFrameQuad(cursorTransform, Cursor, 0, 0);

		if (!mouseClickReady)
		{
			pastTime += ts;
			if (pastTime > mouseClickDelay)
			{
				pastTime = 0.0f;
				mouseClickReady = true;
			}
		}

		if (mouseClickReady)
		{
			if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
			{
				mouseClickReady = false;
				if (mouseOnQuit)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					Gear::Application::Get().Quit();
				}
				if (mouseOnMulti)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
					if (Gear::SceneManager::Get()->isSceneExist("MultiScene"))
					{
						Gear::SceneManager::Get()->changeScene("MultiScene", 0);
					}
					else
					{
						Gear::SceneManager::Get()->AddScene(new MultiScene);
						Gear::SceneManager::Get()->changeScene("MultiScene");
					}
				}
				if (mouseOnNet)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
				}
				if (mouseOnOptions)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
				}
				if (mouseOnSingle)
				{
					PLAY_SOUND_NAME("increaseiconnumber", WormsSound::effect);
				}

				if (!isOnButton)
				{
					PLAY_SOUND_NAME("cantclickhere", WormsSound::effect);
				}
			}
		}
		

		if (inFirst)
		{
			Gear::Renderer2D::DrawQuad(alphaTransform, alphaColor);
			alphaColor.a -= 0.01f;
			if (alphaColor.a <= 0.0f)
			{
				inFirst = false;
			}
		}

		Gear::Renderer2D::EndScene();
	}

	
}