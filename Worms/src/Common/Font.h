#pragma once

namespace FontType
{
	enum Type : unsigned int
	{
		Red,
		RedSmall,
		Blue,
		BlueSmall,
		White,
		WhiteSmall,
		GraySmall,
		RedNumber,
		GrayNumber,
		WhiteNumber,
		FontEnd
	};
}

class Font
{
public:
	static void Init();
	static void ShoutDown();
	static void printFont(const glm::vec2& midPosition, const glm::vec3& scale, const std::string& str, FontType::Type font, float offset);

private:
	static std::vector<Gear::Ref<Gear::FrameTexture2D>> m_Fonts;
	static std::unordered_map<char, int> m_Indexer;
	static glm::vec3 m_Scale;
};
