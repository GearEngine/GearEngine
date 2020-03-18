#pragma once
#include "Gear.h"

#include "InGame/Data/InitiateData.h"

namespace InGame {

	/*class Mask
	{
	public:
		static void SetMask(std::string maskName, const glm::vec3& position, const glm::vec2& scale);

	public:
		static Gear::Ref<Gear::Texture2D> s_Mask;
		static glm::mat4 s_MaskTranslate;
	};*/

	class Terrain
	{
	public:
		Terrain(const InitiateData& initData);

	private:

		int m_ID;

	};

}
