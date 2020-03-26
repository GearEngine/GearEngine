#pragma once

#include "OrthographicCamera.h"
#include "Animation.h"

namespace Gear {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawTextureWithMask(const glm::mat4& translate, const Ref<Texture>& texture, const Ref<Texture>& mask);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& translate, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& translate, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::mat4& translate, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::mat4& translate, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawAnimationQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Animation2D> animation, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawAnimationQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Animation2D> animation, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawAnimationRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Animation2D> animation, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawAnimationRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Animation2D> animation, const glm::vec4& tintColor = glm::vec4(1.0f));
	
		static void DrawAnimation(const glm::mat4& translate, const Ref<Animation2D> animation, const glm::vec4& tintColor = glm::vec4(1.0f));
	
		static void DrawFixedQuad(const glm::mat4& translate, const Ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawFixedQuad(const glm::mat4& translate, const Ref<FrameTexture2D>& frameTexture, int frameX, int frameY, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawFixedAnimation(const glm::mat4& translate, const Ref<Animation2D>& animation, const glm::vec4& tintColor = glm::vec4(1.0f));
		//static void DrawFixedAnimationWithMask(const glm::mat4& textureTranslate, const Ref<Animation2D>& animation, const glm::mat4& maskTranslate, const Ref<Texture>& mask, const glm::vec4& tintColor = glm::vec4(1.0f));
	
		//Batch
		static void BatchDrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void Flush();
	};
}