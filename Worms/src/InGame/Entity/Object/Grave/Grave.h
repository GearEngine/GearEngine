#pragma once

namespace InGame {

	class Grave
	{
	public:
		Grave(const InitiateData& initData);

	private:
		int m_ID;
	};

	class GravePool
	{
	public:
		void init(const InitiateData& initData);
		void init(GraveInfo::Type graveType , const glm::vec2& wolrdPosition);

		static GravePool* Get();
		static void Destroy();

	private:

		static GravePool* s_Inst;

		std::vector<int> GraveType1;
		std::vector<int> GraveType2;
		std::vector<int> GraveType3;
		std::vector<int> GraveType4;
		std::vector<int> GraveType5;
		std::vector<int> GraveType6;

		int GraveType1Ptr = 0;
		int GraveType2Ptr = 0;
		int GraveType3Ptr = 0;
		int GraveType4Ptr = 0;
		int GraveType5Ptr = 0;
		int GraveType6Ptr = 0;
	};

	#define GRAVE_POOL GravePool::Get()

}
