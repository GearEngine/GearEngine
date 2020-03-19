#pragma once
#include "Gear.h"

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

		glm::vec2 m_Mouse;
		glm::vec2 m_MouseOnWorld;
		glm::vec2 m_MouseToLocalTexture;
		glm::vec3 m_Pixel;
		float m_Radius = 20.0f;
	};

}

