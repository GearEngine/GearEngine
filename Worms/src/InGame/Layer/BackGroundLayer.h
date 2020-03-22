#pragma once

#include "InGame/Entity/BackGround/Terrain/Terrain.h"
#include "InGame/Data/InitiateData.h"

namespace InGame {

	struct Leaf
	{
		Leaf() {}
		Leaf(const glm::vec3& position, const glm::vec3& scale, float windResistance)
			: Position(position), Scale(scale), WindResistance(windResistance)
		{}
		glm::vec3 Position;
		glm::mat4 Translate;
		glm::vec3 Scale;

		const float Gravity = 5.0f;
		float Wind;
		float WindResistance;

		void Update(Gear::Timestep ts);
		inline void RenewWind(const float wind) { Wind = wind;  }
	};

	struct Cloud
	{
		Cloud() {}
		Cloud(const glm::vec3& position, const glm::vec3& scale, float basicMove)
			: Position(position), Scale(scale), BasicMove(basicMove)
		{}

		glm::vec3 Position;
		glm::mat4 Translate;
		glm::vec3 Scale;
		
		float BasicMove;
		float Wind;

		void Update(Gear::Timestep ts);
		inline void RenewWind(float wind) { Wind = wind; }
	};


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
		void GradSettup(const InitiateData& initData);
		void WaterSettup(const InitiateData& initData);
		void FallenLeafSettup(const InitiateData& initData);
		void CloudSettup(const InitiateData& initData);

	private:
		static float s_CurrentWind;		//wind -30 ~ 30

		Gear::Ref<Terrain> m_Terrian;
		Gear::Ref<Gear::Texture2D> m_Grad;
		Gear::Ref<Gear::Texture2D> m_TerrainBack;
		Gear::Ref<Gear::Animation2D> m_Water;
		std::vector<Gear::Ref<Gear::Animation2D>> m_LeafTextures;
		std::vector<Gear::Ref<Gear::Animation2D>> m_CloudTextures; // 0 Back, 1 middle, 2 frontSmall

		glm::mat4 m_GradTranslate;
		glm::mat4 m_WaterBottomTranslate;
		glm::vec4 m_WaterBottomColor;
		std::vector<glm::mat4> m_WaterTranslates;
		std::vector<std::vector<Leaf>> m_LeafTranslates;
		std::vector<std::vector<Cloud>> m_CloudTranslates;
	};

}

