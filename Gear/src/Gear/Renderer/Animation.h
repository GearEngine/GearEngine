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
		void Bind(uint32_t slot = 0) const;
		
		void Update(Timestep ts);
		void Start();
		void Stop();
		void Pause();
		void Resume();

		inline void SetFrameX(uint32_t frameX) { m_CurrentFrameX = frameX; }
		inline void SetFrameY(uint32_t frameY) { m_CurrentFrameY = frameY; }
		inline const uint32_t GetFrameX() const { return m_CurrentFrameX; }
		inline const uint32_t GetFrameY() const { return m_CurrentFrameY; }

	private:
		float pastTime = 0.0f;

	private:
		bool m_Loop = false;
		bool m_OnAnimation = false;

		float m_Fps;
		Ref<FrameTexture2D> m_Sprite;

		uint32_t m_MaxFrameX;
		uint32_t m_MaxFrameY;
		uint32_t m_CurrentFrameX;
		uint32_t m_CurrentFrameY;

		bool m_HasFrameOrder = false;
		std::vector<std::pair<int, int>> m_FrameOrder;
		int m_FrameOrderIndex;
	};
}