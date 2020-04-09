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

	namespace Explosion {

		enum Size : unsigned int
		{
			Size25,
			Size50,
			Size75,
			Size100,
		};

		enum Text : unsigned int
		{
			Foom,
			Biff,
			Poot,
			Pow
		};
	}

	
	class Smoke
	{

	};

	class Flame
	{


	};

	class ExplosionSmoke
	{

	};

	class ExplosionEffect
	{
	private:
		void init(Explosion::Size size, Explosion::Text explosionText);

	public:
		void init(const glm::vec2& basePosition);
		
		void Update(float ts);
		void Render();

		bool m_OnUsing = false;
	private:
		int size;

		bool m_InFisrt = true;

		const float m_FrameDelay = 0.02f;
		float m_pastTime = 0.0f;

		Gear::Ref<Gear::FrameTexture2D> m_Circle;
		Gear::Ref<Gear::FrameTexture2D> m_Ellipse;
		Gear::Ref<Gear::FrameTexture2D> m_Text;

		int m_StartCircleIndex;
		int m_StartEllipseIndex;
		int m_StartTextIndex;

		int m_CircleIndex;
		int m_EllipseIndex;
		int m_TextIndex;

		glm::vec3 m_CircleScale;
		glm::vec3 m_EllipseScale;
		glm::vec3 m_TextScale;

		glm::mat4 m_CircleTranslate;
		glm::mat4 m_EllipseTranslate;
		glm::mat4 m_TextTranslate;		
		glm::vec2 m_BasePosition;

		friend class EffectPool;
	};	

#define EFFECT_POOL_MAX 300
	class EffectPool
	{
	public:
		static void Init();
		static Gear::Ref<Blob> GetBlob();
		static Gear::Ref<ExplosionEffect> GetExplosion(Explosion::Size size, Explosion::Text text);

	public:
		static std::vector<Gear::Ref<Blob>> s_BlobPool;
		static int s_BlobPtr;
		
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex25Foom;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Foom;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Foom;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Foom;

		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex25Biff;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Biff;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Biff;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Biff;

		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex25Poot;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Poot;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Poot;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Poot;
		
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex25Pow;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Pow;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Pow;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Pow;

		static int s_Ex25FoomPtr;
		static int s_Ex50FoomPtr;
		static int s_Ex75FoomPtr;
		static int s_Ex100FoomPtr;

		static int s_Ex25BiffPtr;
		static int s_Ex50BiffPtr;
		static int s_Ex75BiffPtr;
		static int s_Ex100BiffPtr;

		static int s_Ex25PootPtr;
		static int s_Ex50PootPtr;
		static int s_Ex75PootPtr;
		static int s_Ex100PootPtr;

		static int s_Ex25PowPtr;
		static int s_Ex50PowPtr;
		static int s_Ex75PowPtr;
		static int s_Ex100PowPtr;
	};


}