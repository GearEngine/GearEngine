#pragma once

namespace InGame {

	namespace FontType
	{
		enum Type: unsigned int
		{
			ColorRed,
			ColorBlue,
		};
	}

	class Font
	{
	public:
		static void Init();
		static void Destroy();
		static void printFont(const glm::vec2& startPosition, const std::string& str, FontType::Type font);

	private:
		static std::vector<Gear::FrameTexture2D> m_Fonts;
	};

}