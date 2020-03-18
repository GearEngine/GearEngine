#pragma once

#include "Gear/Renderer/Texture.h"
#include "Gear/Renderer/OrthographicCameraController.h"

namespace Gear {

	class Coord2DManger
	{
	public:
		static Coord2DManger* Get();
		static void Init();
		static void Destroy();

		inline void SetCamera(OrthographicCameraController* cameraController) { m_CameraController = cameraController; }
		void SetResolution(float windowWidth, float windowHeight);

		virtual glm::vec4 GetPixel_From_WorldPosition(const glm::vec2& worldPosition) = 0;
		virtual glm::vec4 GetPixel_From_ScreenPosition(const glm::vec2& worldPosition) = 0;
		virtual glm::vec2 GetWorldPosition_From_ScreenPosition(const glm::vec2& screenPosition) = 0;
		virtual glm::vec2 GetScreenPosition_From_WorldPosition(const glm::vec2& worldPosition) = 0;
		virtual glm::vec2 GetTextureLocalPosition_From_WorlPosition(const glm::vec2& worldPosition, const glm::mat4& textureTranslate) = 0;
		virtual glm::vec2 GetTextureLocalPosition_From_ScreenPosition(const glm::vec2& worldPosition, const glm::mat4& textureTranslate) = 0;

		virtual glm::vec4 GetPixel_From_TextureLocal_With_WorldPosition(Ref<Texture2D> texture, const glm::vec2& worldPosition, const glm::mat4& textureTranslate) = 0;
		virtual glm::vec4 GetPixel_From_TextureLocal_With_ScreenPosition(Ref<Texture2D> texture, const glm::vec2& screenPosition, const glm::mat4& textureTranslate) = 0;
		virtual glm::vec4 GetPixel_From_TextureLocal_With_TextureLocalPosition(Ref<Texture2D> texture, const glm::vec2& textureLocalPosition) = 0;

	protected:
		OrthographicCameraController* m_CameraController = nullptr;
		glm::vec2 m_WindowResolution;
		float m_AspectRatio;

	private:
		static Coord2DManger* s_Inst;
	};

}