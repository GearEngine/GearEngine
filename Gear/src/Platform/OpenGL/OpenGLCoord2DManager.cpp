#include "grpch.h"
#include "OpenGLCoord2DManager.h"

#include <glad/glad.h>

namespace Gear {

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_WorldPosition(const glm::vec2& worldPosition)
	{
		glm::vec2 screenPosition = GetScreenPosition_From_WorldPosition(worldPosition);
		return GetPixel_From_ScreenPosition(screenPosition);
	}

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_ScreenPosition(const glm::vec2& screenPosition)
	{
		glm::vec4 pixel;
		unsigned char pixelBuffer[4];
		glReadPixels(screenPosition.x, screenPosition.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
		pixel.r = pixelBuffer[0];
		pixel.g = pixelBuffer[1];
		pixel.b = pixelBuffer[2];
		pixel.a = pixelBuffer[3];
		return pixel;
	}

	glm::vec2 OpenGLCoord2DManager::GetWorldPosition_From_ScreenPosition(const glm::vec2& screenPosition)
	{
		glm::vec3 worldPosition;
		auto& camera = m_CameraController->GetCamera();
		auto& cameraPos = camera.GetPosition();

		worldPosition = glm::inverse(camera.GetViewProjectionMatrix()) * glm::vec4(screenPosition, 0.0f, 1.0f);

		worldPosition.x /= m_WindowResolution.x * 0.5;
		worldPosition.y /= m_WindowResolution.y * 0.5;
		worldPosition.x += cameraPos.x - m_AspectRatio * m_CameraController->GetZoomLevel();
		worldPosition.y += cameraPos.y - 1.0f * m_CameraController->GetZoomLevel();

		return glm::vec2(worldPosition.x, worldPosition.y);
	}

	glm::vec2 OpenGLCoord2DManager::GetScreenPosition_From_WorldPosition(const glm::vec2& worldPosition)
	{
		auto& camera = m_CameraController->GetCamera();
		glm::vec3 screenPosition = camera.GetViewProjectionMatrix() * glm::vec4({ worldPosition, 0.0f, 1.0f });

		return glm::vec2(screenPosition.x, screenPosition.y);
	}

	glm::vec2 OpenGLCoord2DManager::GetTextureLocalPosition_From_ScreenPosition(const glm::vec2& screenPosition, const glm::mat4& textureTranslate)
	{
		auto worldPosition = GetWorldPosition_From_ScreenPosition(screenPosition);
		return GetTextureLocalPosition_From_WorlPosition(worldPosition, textureTranslate);
	}

	glm::vec2 OpenGLCoord2DManager::GetTextureLocalPosition_From_WorlPosition(const glm::vec2& worldPosition, const glm::mat4& textureTranslate)
	{
		glm::vec3 localPosition;
		auto inverse = glm::inverse(textureTranslate);
		localPosition = inverse * glm::vec4(worldPosition, 0.0f, 1.0f);

		localPosition.x += 0.5;
		localPosition.y += 0.5;

		return glm::vec2(localPosition.x, localPosition.y);
	}

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_TextureLocal_With_WorldPosition(Ref<Texture2D> texture, const glm::vec2& worldPosition, const glm::mat4& textureTranslate)
	{
		auto textureLocalPosition = GetTextureLocalPosition_From_WorlPosition(worldPosition, textureTranslate);
		return GetPixel_From_TextureLocal_With_TextureLocalPosition(texture, textureLocalPosition);
	}

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_TextureLocal_With_ScreenPosition(Ref<Texture2D> texture, const glm::vec2& screenPosition, const glm::mat4& textureTranslate)
	{
		auto textureLocalPosition = GetTextureLocalPosition_From_ScreenPosition(screenPosition, textureTranslate);
		return GetPixel_From_TextureLocal_With_TextureLocalPosition(texture, textureLocalPosition);
	}

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_TextureLocal_With_TextureLocalPosition(Ref<Texture2D> texture, const glm::vec2 & textureLocalPosition)
	{
		glm::vec4 pixel;
		int result;

		unsigned int* data = (unsigned int*)texture->GetData();
		int width = texture->GetWidth();
		int height = texture->GetHeight();

		int xPos = width * textureLocalPosition.x;
		int yPos = height * textureLocalPosition.y;

		if (xPos >= width || xPos < 0 || yPos >= height || yPos < 0)
		{
			return pixel = { 0.0f, 0.0f, 0.0f, 0.0f };
		}

		result = data[yPos * width + xPos];
		unsigned char* divider = (unsigned char*)&result;
		pixel.r = divider[0];
		pixel.g = divider[1];
		pixel.b = divider[2];
		pixel.a = divider[3];

		return pixel;
	}

}