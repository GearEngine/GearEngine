#pragma once
#include "Gear/Manager/CoordManager.h"
#include "Gear/Renderer/Texture.h"

namespace Gear {

	class OpenGLCoord2DManager : public Coord2DManger
	{
	public:
		virtual glm::vec4 GetPixel_From_WorldPosition(const glm::vec2& worldPosition) override;
		virtual glm::vec4 GetPixel_From_ScreenPosition(const glm::vec2& screenPosition) override;

		virtual glm::vec2 GetWorldPosition_From_ScreenPosition(const glm::vec2& screenPosition) override;
		virtual glm::vec2 GetScreenPosition_From_WorldPosition(const glm::vec2& worldPosition) override;

		virtual glm::vec2 GetTextureLocalPosition_From_WorlPosition(const glm::vec2& worldPosition, const glm::mat4& textureTranslate) override;
		virtual glm::vec2 GetTextureLocalPosition_From_ScreenPosition(const glm::vec2& screenPosition, const glm::mat4& textureTranslate) override;
		
		virtual glm::vec4 GetPixel_From_TextureLocal_With_WorldPosition(Ref<Texture2D> texture, const glm::vec2& worldPosition, const glm::mat4& textureTranslate) override;
		virtual glm::vec4 GetPixel_From_TextureLocal_With_ScreenPosition(Ref<Texture2D> texture, const glm::vec2& screenPosition, const glm::mat4& textureTranslate) override;
		virtual glm::vec4 GetPixel_From_TextureLocal_With_TextureLocalPosition(Ref<Texture2D> texture, const glm::vec2& textureLocalPosition) override;
		
		virtual glm::vec3 GetPixel_From_TextureLocal_With_TextureRealPosition(Ref<Texture2D> texture, const std::pair<int, int>& textureRealPosition) override;
	
	};
}