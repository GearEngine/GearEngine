#pragma once

namespace InGame {

	class Blob
	{
	public:
		Blob() = default;	
	public:
		void reset(const glm::vec3& basePosition, int count);
		void UpDate(float tick);
		void Render();
		void SetAngle(float angle);

		bool m_OnUsing = false;
	private:
		Gear::Ref<Gear::FrameTexture2D> m_BlobTexture;
		glm::vec3 m_Position;
		glm::vec3 m_Scale = { 1.8f, 1.8f, 1.0f };
		glm::vec3 m_BasePosition;
		glm::mat4 m_Translate;
		float m_Angle;
		float m_Offset = 0.0f;
		const float m_LimitOffset = 2.8f;
		const float m_OffsetSpeed = 1.4f;
		int m_TextureIndex = 15;
		const float m_FrameDelay = 0.1f;
		float m_PastTime = 0.0f;

		friend class EffectPool;
	};

	class ExplosionSmall
	{

	};


#define EFFECT_POOL_MAX 300
	class EffectPool
	{
	public:
		static void Init();
		static Gear::Ref<Blob> GetBlob();

	public:
		static int s_BlobPtr;
		static std::vector<Gear::Ref<Blob>> s_BlobPool;
	};


}