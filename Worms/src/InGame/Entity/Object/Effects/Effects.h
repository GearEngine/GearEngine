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
			Pow,
			Null
		};

		enum SmokeSize : unsigned int
		{
			Size20,
			Size30,
			Size40
		};
	}

	namespace ExhaustType
	{
		enum Type : unsigned int
		{
			_1,
			Magic
		};
	}

	class Spangle
	{
	private:
		void init(TeamColor::Color teamColor);
	public:
		void init(const glm::vec2& worldPosition);
		void Update(Gear::Timestep ts);
		void Render();

		glm::vec3 m_Scale;
		glm::mat4 m_Transform;

		Gear::Ref<Gear::FrameTexture2D> m_Texture;
		int m_StartIndex;
		int m_TextureIndex;

		const float m_FrameDelay = 0.01f;
		float m_PastTime = 0.0f;
		bool m_OnUsing = false;

		friend class EffectPool;
	};
	
	class Marker
	{
	private:
		void init(TeamColor::Color teamColor);
	public:
		void init(const glm::vec2& worldPosition);
		void Update(Gear::Timestep ts);
		void Render();

		glm::vec3 m_Scale;
		glm::mat4 m_Transform;

		Gear::Ref<Gear::FrameTexture2D> m_Texture;
		int m_StartIndex;
		int m_TextureIndex;

		const float m_FrameDelay = 0.01f;
		float m_PastTime = 0.0f;
		int m_LoopCount = 0;
		bool m_OnUsing = false;
		bool m_OnReverse = false;

		friend class EffectPool;
	};

	class Smoke
	{
	private:
		void init(Explosion::SmokeSize size);
	public:
		void init(const glm::vec2& worldPosition, const glm::vec2& externalVector = glm::vec2(0.0f, 0.0f));
		void Update(float ts);
		void Render();

		bool m_OnUsing = false;

	private:
		glm::vec3 m_Scale;
		glm::mat4 m_OriginTranslate;
		glm::mat4 m_Translate;
		glm::vec2 m_ExternalVector;

		Gear::Ref<Gear::FrameTexture2D> m_Texture;
		int m_StartIndex;
		int m_TextureIndex;

		const float m_FrameDelay = 0.01f;
		float m_PastTime = 0.0f;

		friend class EffectPool;
	};

	class Exhaust
	{
	public:
		void init(ExhaustType::Type type);
		void init(const glm::vec3& wolrdPosition);

		void Update(float ts);
		void Render();

		bool m_OnUsing = false;

	private:
		Gear::Ref<Gear::FrameTexture2D> m_Texture;
		glm::vec3 m_Scale;
		glm::mat4 m_Translate;
		glm::mat4 m_OriginTranslate;

		int m_StartIndex;
		int m_TextureIndex;

		const float m_FrameDelay = 0.01f;
		float m_PastTime = 0.0f;

		friend class EffectPool;
	};

	class FlameBundle
	{
	public:
		class Flame
		{
		public:
			void init(const glm::vec2& externalVector, Explosion::Size size);
			void init(const glm::vec2& wolrdPosition);

			void Update(float ts);
			void Render();

			bool m_OnUsing = false;

		private:
			Explosion::Size m_Size;

			glm::vec3 m_Position;
			glm::vec3 m_Scale;
			glm::vec2 m_OriginExternalVector;
			glm::vec2 m_ExternalVector;

			float m_Gravity = 0.5f;
			int m_StartIndex;

			float m_GenSmokeDelay;
			const float m_FrameDelay = 0.08f;
			float m_PastTime = 0.0f;
			float m_GenPastTime = 0.0f;

			Gear::Ref<Gear::FrameTexture2D> m_Texture;
			glm::mat4 m_OriginTranslate;
			glm::mat4 m_Translate;

			int m_TextureIndex;
		};

		bool m_OnUsing = false;

	private:
		void init(Explosion::Size size);
	public:
		void init(const glm::vec2& worldPosition);
		void Update(float ts);
		void Render();

	private:
		std::vector<Flame> m_Flames;
		Explosion::Size m_Size;

		friend class EffectPool;
	};

	

	class ExplosionSmokeBunddle
	{
	private:
		class ExplosionSmoke
		{
		public:
			void init(Explosion::Size size, const glm::vec2& offsetVector, const glm::vec2 & externalVector);
			void init(const glm::vec2& wolrdPosition);

			void Update(float ts);
			void Render();

			bool m_OnUsing = false;
		private:
			glm::vec3 m_Position;
			glm::vec3 m_Scale;
			glm::vec2 m_OriginExternalVector;
			glm::vec2 m_ExternalVector;
			int m_StartIndex;

			const float m_FrameDelay = 0.02f;
			float m_PastTime = 0.0f;

			Gear::Ref<Gear::FrameTexture2D> m_Texture;
			glm::mat4 m_OriginTranslate;
			glm::mat4 m_Translate;

			int m_TextureIndex;
		};

	private:
		void init(Explosion::Size size);

	public:
		void init(const glm::vec2& basePosition);

		void Update(float ts);
		void Render();

		bool m_OnUsing = false;
	private:
		std::vector<ExplosionSmoke> m_Smokes;

		friend class EffectPool;
	};

	class ExplosionEffect
	{
	private:
		void init(Explosion::Size size, Explosion::Text explosionText);

	public:
		//NOTE : specificSound can be your soundfile name or "None", if you not assign name play genral explosion sound randomly
		void init(const glm::vec2& basePosition, const std::string& specificSound = "");
		
		void Update(float ts);
		void Render();

		bool m_OnUsing = false;
	private:
		bool m_InFisrt = true;

		float m_FrameDelay;
		float m_pastTime = 0.0f;

		float m_TextFrameDelay = 0.02f;
		float m_TextPastTime = 0.0f;

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
		static Gear::Ref<ExplosionSmokeBunddle> GetExplosionSmoke(Explosion::Size size);
		static Gear::Ref<FlameBundle> GetFlame(Explosion::Size size);
		static Gear::Ref<Smoke> GetSmoke(Explosion::SmokeSize size);
		static Gear::Ref<Exhaust> GetExhuast(ExhaustType::Type type);
		static Gear::Ref<Spangle> GetSpangle(TeamColor::Color color);
		static Gear::Ref<Marker> GetMarker(TeamColor::Color color);

		static void pushExplosion(Gear::Ref<ExplosionEffect> explosion);
		static void pushExplosionSmoke(Gear::Ref<ExplosionSmokeBunddle> explosionSmoke);
		static void pushFlame(Gear::Ref<FlameBundle> flame);
		static void pushSpangle(Gear::Ref<Spangle> spangle);
		static void pushMarker(Gear::Ref<Marker> spangle);
		static void pushExhaust(Gear::Ref<Exhaust> exhaust);

	public:
		static int explosionMax;
		static int flameMax;
		static bool initiated;

	public:
		static std::vector<Gear::Ref<Blob>> s_BlobPool;
		static int s_BlobPtr;
		
		static std::vector<Gear::Ref<Spangle>> s_Spangle;
		static std::vector<Gear::Ref<Marker>> s_Marker;
		
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex25;

		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Foom;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Foom;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Foom;

		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Biff;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Biff;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Biff;

		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Poot;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Poot;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Poot;
		
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex50Pow;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex75Pow;
		static std::vector<Gear::Ref<ExplosionEffect>> s_Ex100Pow;

		static int s_Ex25Ptr;

		static int s_Ex50FoomPtr;
		static int s_Ex75FoomPtr;
		static int s_Ex100FoomPtr;

		static int s_Ex50BiffPtr;
		static int s_Ex75BiffPtr;
		static int s_Ex100BiffPtr;

		static int s_Ex50PootPtr;
		static int s_Ex75PootPtr;
		static int s_Ex100PootPtr;

		static int s_Ex50PowPtr;
		static int s_Ex75PowPtr;
		static int s_Ex100PowPtr;

		static std::vector<Gear::Ref<ExplosionSmokeBunddle>> s_Sm25;
		static std::vector<Gear::Ref<ExplosionSmokeBunddle>> s_Sm50;
		static std::vector<Gear::Ref<ExplosionSmokeBunddle>> s_Sm75;
		static std::vector<Gear::Ref<ExplosionSmokeBunddle>> s_Sm100;

		static int s_Sm25Ptr;
		static int s_Sm50Ptr;
		static int s_Sm75Ptr;
		static int s_Sm100Ptr;

		static std::vector<Gear::Ref<FlameBundle>> s_Fm25;
		static std::vector<Gear::Ref<FlameBundle>> s_Fm50;
		static std::vector<Gear::Ref<FlameBundle>> s_Fm75;
		static std::vector<Gear::Ref<FlameBundle>> s_Fm100;

		static int s_Fm25Ptr;
		static int s_Fm50Ptr;
		static int s_Fm75Ptr;
		static int s_Fm100Ptr;

		static std::vector<Gear::Ref<Smoke>> s_Sm20;
		static std::vector<Gear::Ref<Smoke>> s_Sm30;
		static std::vector<Gear::Ref<Smoke>> s_Sm40;

		static int s_Sm20Ptr;
		static int s_Sm30Ptr;
		static int s_Sm40Ptr;

		static std::vector<Gear::Ref<Exhaust>> s_Exhaust1;
		static std::vector<Gear::Ref<Exhaust>> s_ExhaustMagic;

		static int s_Exhaust1Ptr;
		static int s_ExhaustMagicPtr;

	};
}