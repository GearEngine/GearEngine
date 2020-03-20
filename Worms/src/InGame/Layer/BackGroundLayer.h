#pragma once

#include "InGame/Entity/BackGround/Terrain/Terrain.h"
#include "InGame/Data/InitiateData.h"

namespace InGame {

	class BackGroundLayer : public Gear::Layer
	{
	public:
		BackGroundLayer(const InitiateData& initData);
		virtual ~BackGroundLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Gear::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Gear::Event& e) override;

		//void DestroyMask(float x, float y, float radius);

	private:
		Gear::Ref<Terrain> m_Terrian;
		Gear::Ref<Gear::Texture2D> m_Grad;
		Gear::Ref<Gear::Animation2D> m_Water;

		glm::mat4 m_GradTranslate;
		glm::mat4 m_WaterBottomTranslate;
		glm::vec4 m_WaterBottomColor;
		std::vector<glm::mat4> m_WaterTranslates;

		/*glm::vec2 m_Mouse;
		glm::vec2 m_MouseOnWorld;
		glm::vec2 m_MouseToLocalTexture;
		glm::vec3 m_Pixel;
		float m_Radius = 20.0f;*/
	};

}

