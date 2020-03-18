#pragma once
#include "Gear/Manager/CoordManager.h"

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
	};



}