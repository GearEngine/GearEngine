#pragma once

#include "InGame/Layer/BackGroundLayer.h"
#include "InGame/Layer/ObjectLayer.h"
#include "InGame/Layer/UILayer.h"

namespace InGame {

	/*inline void workFunk(InitiateData initData) 
	{
		int i = 1;
		auto tempCamera = new Gear::OrthographicCameraController(initData.WindowWidth / initData.WindowHeight);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		tempCamera->GetCamera().SetPosition({ 0.0f, 0.0f, 1.0f });
		while (++i)
		{
			Gear::RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f });
			Gear::RenderCommand::Clear();

			Gear::Renderer2D::BeginScene(tempCamera->GetCamera());

			Gear::Renderer2D::DrawQuad(trans, Gear::TextureStorage::GetTexture2D("City"));

			Gear::Renderer2D::EndScene();

			Gear::Application::Get().GetWindow2().OnUpdate();

			if (i == 300)
				return;
		}
	}*/

	class InGameScene : public Gear::Scene
	{
	public:
		InGameScene(const std::string& name, Gear::Layer* imGuiLayer, const InitiateData& initData)
			: Gear::Scene(name)
		{
			//std::thread LoadingThread = std::thread(workFunk, initData);
			//LoadingThread.join();

			std::thread InitThread = std::thread([&]() {
				PushLayer(new UILayer(initData));
				PushLayer(new BackGroundLayer(initData));
				PushLayer(new ObjectLayer(initData));
			});

			InitThread.join();
		}

		inline virtual bool PassFirstDelay() override { return m_PassFistDelay; }
		inline virtual void Update(Gear::Timestep ts) override
		{
			/*if (!m_PassFistDelay)
			{
				m_ElapsedTime += ts;
				if (m_StartDelay < m_ElapsedTime)
				{
					m_PassFistDelay = true;
				}
				return;
			}*/

			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate(ts);
			}
		}

	public:
		bool m_PassFistDelay = true;

	private:
		float m_StartDelay = 2.0f;
		float m_ElapsedTime = 0.0f;
	};

}
