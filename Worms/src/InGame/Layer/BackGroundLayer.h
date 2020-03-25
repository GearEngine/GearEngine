#pragma once

#include "InGame/Entity/BackGround/Terrain/Terrain.h"
#include "InGame/Entity/BackGround/Terrain/TerrianBack.h"

namespace InGame {

	struct FloatingMatter
	{
		FloatingMatter() {}
		FloatingMatter(const glm::vec3& position, const glm::vec3& scale, float windResistance, const Gear::Util::FRect& worldRect)
			: Position(position), Scale(scale), WindResistance(windResistance), WorldRect(worldRect)
		{}
		glm::vec3 Position;
		glm::mat4 Translate;
		glm::vec3 Scale;

		const float Gravity = 5.0f;
		float WindResistance;
		Gear::Util::FRect WorldRect;

		void Update(Gear::Timestep ts);
	};

	struct Cloud
	{
		Cloud() {}
		Cloud(const glm::vec3& position, const glm::vec3& scale, float basicMove, const Gear::Util::FRect& worldRect)
			: Position(position), Scale(scale), BasicMove(basicMove), WorldRect(worldRect)
		{}

		glm::vec3 Position;
		glm::mat4 Translate;
		glm::vec3 Scale;
		
		float BasicMove;
		Gear::Util::FRect WorldRect;

		void Update(Gear::Timestep ts);
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
		inline static void ResetCurrentWind();
		inline static int GetCurrentWind() { return s_CurrentWind; }

	private:
		void GradSettup(const InitiateData& initData);
		void WaterSettup(const InitiateData& initData);
		void FloatingSettup(const InitiateData& initData);
		void CloudSettup(const InitiateData& initData);

	private:
		static float s_CurrentWind;		//wind -30 ~ 30
		static float s_SettedWind;
		static int s_WindMax;

		Gear::Ref<Terrain> m_Terrian;
		Gear::Ref<TerrianBack> m_TerrianBack;
		Gear::Ref<Gear::Texture2D> m_Grad;
		Gear::Ref<Gear::Animation2D> m_WaterWave;
		std::vector<Gear::Ref<Gear::Animation2D>> m_FloatingTextures;
		std::vector<Gear::Ref<Gear::Animation2D>> m_CloudTextures; // 0 Back, 1 middle, 2 frontSmall

		glm::mat4 m_GradTranslate;
		glm::mat4 m_WaterBackGroundTranslate;
		glm::vec4 m_WaterBackGroundmColor;
		std::vector<glm::mat4> m_WaterWaveTranslates;
		std::vector<std::vector<FloatingMatter>> m_FloatingTranslates;
		std::vector<std::vector<Cloud>> m_CloudTranslates;

		int m_Transceiver; // Entity
	};

	class BgLayerTransceiver : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			auto worldData = std::any_cast<WorldData>(data);
			if (worldData.DataType == WorldDataType::NewStart)
			{
				BackGroundLayer::ResetCurrentWind();
			}
		}
	};

}

