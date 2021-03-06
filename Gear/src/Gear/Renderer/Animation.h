#pragma once

#include "Texture.h"
#include "Gear/Core/Timestep.h"

namespace Gear {

	class Animation
	{
	public:
		virtual ~Animation() = default;

		virtual void Update(Timestep ts) = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
	};

	class Animation2D : public Animation
	{
	public:
		Animation2D(Ref<FrameTexture2D> sprite, float fps, bool loop);
		Animation2D(Ref<FrameTexture2D> sprite, float fps, const std::vector<std::pair<int, int>> frameOrder, bool loop);
		
	public:
		static Ref<Animation2D> Create(Ref<FrameTexture2D> sprite, float fps, bool loop = false);
		static Ref<Animation2D> Create(Ref<FrameTexture2D> sprite, float fps, const std::vector<std::pair<int, int>> frameOrder, bool loop = false);

	public:
		virtual void Update(Timestep ts) override;
		virtual void Start() override;
		virtual void Stop() override;
		virtual void Pause() override;
		virtual void Resume() override;
		inline bool isCompleteOneLoop() { return m_CompleteOneLoop; }
		inline glm::vec4& GetTintColor() { return m_TintColor; }
		inline void SetTintColor(const glm::vec4& tintColor) { m_TintColor = tintColor; }

		void SetFrameX(uint32_t frameX);
		void SetFrameY(uint32_t frameY);
		inline const uint32_t GetFrameX() const { return m_CurrentFrameX; }
		inline const uint32_t GetFrameY() const { return m_CurrentFrameY; }

		void Bind(uint32_t slot = 0) const;

	private:
		float pastTime = 0.0f;

	private:
		bool m_Loop = false;
		bool m_OnAnimation = false;
		bool m_CompleteOneLoop = false;

		float m_Fps = 0.0f;
		Ref<FrameTexture2D> m_Sprite;

		uint32_t m_MaxFrameX;
		uint32_t m_MaxFrameY;
		uint32_t m_CurrentFrameX = 0;
		uint32_t m_CurrentFrameY = 0;

		glm::vec4 m_TintColor = glm::vec4(1.0f);
		bool m_HasFrameOrder = false;
		std::vector<std::pair<int, int>> m_FrameOrder;
		int m_FrameOrderIndex;
	};
}