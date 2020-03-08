#include "grpch.h"
#include "Application.h"

#include "Gear/Core/Log.h"

#include "Gear/Core/SceneManager.h"
#include "Gear/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Gear {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GR_PROFILE_FUNCTION();

		GR_CORE_ASSERT(!s_Instance, "Application arleady exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuilayer = new ImGuiLayer;
	}

	Application::~Application()
	{
		GR_PROFILE_FUNCTION();

		Renderer::Shutdown();
		Gear::SceneManager::Destory();
	}

	void Application::OnEvent(Event & e)
	{
		GR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_CurScene->end(); it != m_CurScene->begin();) {
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		GR_PROFILE_FUNCTION();

		while (m_Running)
		{
			GR_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			
			if (!m_Minimized)
			{
				m_CurScene = Gear::SceneManager::Get()->GetCurrentScene();
				{
					GR_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : *m_CurScene)
						layer->OnUpdate(timestep);
				}

				m_ImGuilayer->Begin();
				{
					GR_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (Layer* layer : *m_CurScene)
						layer->OnImGuiRender();
				}
				m_ImGuilayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent & e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		GR_PROFILE_SCOPE("RunLoop");

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}