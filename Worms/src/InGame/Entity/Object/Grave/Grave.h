#pragma once

namespace InGame {

	class Grave
	{
	public:
		Grave(const InitiateData& initData, GraveInfo::Type type);
		void init(const glm::vec2& wolrdPosition);

	private:
		int m_ID;
		
		friend class GravePool;
	};

	class GravePool
	{
	private:
		~GravePool();

	public:
		void init(const InitiateData& initData);
		void ActivateGrave(GraveInfo::Type type, const glm::vec2& worldPosition);

		static GravePool* Get();
		static void Destroy();

	private:
		static GravePool* s_Inst;

		std::vector<Gear::Ref<Grave>> m_GraveType1;
		std::vector<Gear::Ref<Grave>> m_GraveType2;
		std::vector<Gear::Ref<Grave>> m_GraveType3;
		std::vector<Gear::Ref<Grave>> m_GraveType4;
		std::vector<Gear::Ref<Grave>> m_GraveType5;
		std::vector<Gear::Ref<Grave>> m_GraveType6;

		int m_GraveType1Ptr = 0;
		int m_GraveType2Ptr = 0;
		int m_GraveType3Ptr = 0;
		int m_GraveType4Ptr = 0;
		int m_GraveType5Ptr = 0;
		int m_GraveType6Ptr = 0;

		const int GraveMax = 64;
	};

	#define GRAVE_POOL GravePool::Get()

}
