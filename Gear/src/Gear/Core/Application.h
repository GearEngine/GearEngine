#pragma once

#include "Core.h"

#include "Window.h"
#include "Gear/Manager/SceneManager.h"
#include "Gear/Events/Event.h"
#include "Gear/Events/ApplicationEvent.h"

#include "Gear/Core/Timestep.h"

#include "Gear/ImGui/ImGuiLayer.h"

namespace Gear {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	protected:
		ImGuiLayer* m_ImGuilayer;

	private:
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;
		Scene* m_CurScene;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
