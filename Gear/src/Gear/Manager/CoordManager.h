#pragma once

#include "Gear/Renderer/Texture.h"
#include "Gear/Renderer/OrthographicCameraController.h"

namespace Gear {

	class Coord2DManger
	{
	public:
		static Coord2DManger* Get();
		static void init();

		inline void SetCamera(OrthographicCameraController* cameraController) { m_CameraController = cameraController; }
		inline void SetResolution(float windowWidth, float windowHeight) { m_WindowResolution = { windowWidth, windowHeight }; }

		virtual glm::vec4 GetPixel_From_WorldPosition(glm::vec2 worldPosition) = 0;
		virtual glm::vec4 GetPixel_From_ScreenPosition(glm::vec2 worldPosition) = 0;
		virtual glm::vec2 GetWorldPosition_From_ScreenPosition(glm::vec2 screenPosition) = 0;
		virtual glm::vec2 GetScreenPosition_From_WorldPosition(glm::vec2 worldPosition) = 0;
		virtual glm::vec2 GetTextureLocalPosition_From_WorlPosition(glm::vec2 worldPosition, Ref<Texture2D> texture) = 0;
		virtual glm::vec2 GetTextureLocalPosition_From_ScreenPosition(glm::vec2 worldPosition, Ref<Texture2D> texture) = 0;

	protected:
		OrthographicCameraController* m_CameraController = nullptr;
		glm::vec2 m_WindowResolution;
		float m_AspectRatio;

	private:
		static Coord2DManger* s_Inst;
	};

}