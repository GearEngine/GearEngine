#include "grpch.h"
#include "OpenGLCoord2DManager.h"

#include <glad/glad.h>

namespace Gear {

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_WorldPosition(glm::vec2 worldPosition)
	{
		glm::vec2 screenPosition = GetScreenPosition_From_WorldPosition(worldPosition);
		return GetPixel_From_ScreenPosition(screenPosition);
	}

	glm::vec4 OpenGLCoord2DManager::GetPixel_From_ScreenPosition(glm::vec2 screenPosition)
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

	glm::vec2 OpenGLCoord2DManager::GetWorldPosition_From_ScreenPosition(glm::vec2 screenPosition)
	{
		glm::vec3 worldPosition;
		auto& camera = m_CameraController->GetCamera();
		auto& cameraPos = camera.GetPosition();

		worldPosition = glm::inverse(camera.GetViewProjectionMatrix()) * glm::vec4(screenPosition, 0.0f, 1.0f);

		worldPosition.x /= m_WindowResolution.x * 0.5;
		worldPosition.y /= m_WindowResolution.y * 0.5;
		worldPosition.x += cameraPos.x - m_AspectRatio * m_CameraController->GetZoomLevel();
		worldPosition.y += cameraPos.y - 1.0f * m_CameraController->GetZoomLevel();

		return worldPosition;
	}

	glm::vec2 OpenGLCoord2DManager::GetScreenPosition_From_WorldPosition(glm::vec2 worldPosition)
	{
		auto& camera = m_CameraController->GetCamera();
		glm::vec3 screenPosition = camera.GetViewProjectionMatrix() * glm::vec4({ worldPosition, 0.0f, 1.0f});
		
		return screenPosition;
	}

	glm::vec2 OpenGLCoord2DManager::GetTextureLocalPosition_From_WorlPosition(glm::vec2 worldPosition, Ref<Texture2D> texture)
	{
		return glm::vec2();
	}

	glm::vec2 OpenGLCoord2DManager::GetTextureLocalPosition_From_ScreenPosition(glm::vec2 screenPosition, Ref<Texture2D> texture)
	{
		return glm::vec2();
	}

}